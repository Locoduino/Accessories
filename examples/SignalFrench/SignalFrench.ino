/*************************************************************
project: <DCC Accessory Decoder sample>
author: <Thierry PARIS>
description: <10 leds for a french railroad signal>
*************************************************************/

#include <UniversalAccessoryDecoder.h>

#define NB_LEDS_MAX			20
#define PATTERN_NB_LEDS_MAX	4
#define PATTERN_END_LIST	127

//-------------------------------------------------------------------
// This class defines a light signal by giving patterns of lights:
// like the state -x--BB---- for a signal with 10 lights.
// Where x means light on, - means off, and B is blink.
//-------------------------------------------------------------------

class SignalArduinoPattern : public AccessoryLightMulti
{
private:
	const byte *pPatterns;
	const byte *pRealStates;
	int startingDcc;
	int blinkDuration;

public:
	SignalArduinoPattern(DriverArduino *inpDriver, byte inNbLeds, const int *inpPins, int inFirstPort = 0);
	void SetupSignal(int inStartingDcc, int inBlinkDuration, const byte *inPatterns, const byte *inpRealStates = 0);
	void Move(int inDccId, byte inDccAccessory);

	static int GetStatesNumber(const byte *pStates);
};

SignalArduinoPattern::SignalArduinoPattern(DriverArduino *inpDriver, byte inNbLeds, const int *inpPins, int inFirstPort) : AccessoryLightMulti(0, 0, inNbLeds, 0)
{
	for (int led = 0; led < inNbLeds; led++)
	{
		inpDriver->SetupPortMotor(inFirstPort + led, inpPins[led], DIGITAL);
	}

	for (int led = 0; led < inNbLeds; led++)
	{
		this->SetupLight(led, inpDriver, inFirstPort + led, 255);
	}
}

void SignalArduinoPattern::SetupSignal(int inStartingDcc, int inBlinkDuration, const byte *inPatterns, const byte *inpRealStates)
{
	this->startingDcc = inStartingDcc;
	this->blinkDuration = inBlinkDuration;
	this->pPatterns = inPatterns;
	this->pRealStates = inpRealStates;

	this->Setup();

	// Pass through all states during 5s to check
	for (int i = 0; i < this->GetStatesNumber(this->pRealStates); i++)
	{
		this->Move(this->startingDcc + (i / 2), i % 2);
		unsigned long start = millis();
		while (millis() - start < 5000)
			this->ActionEnded();

		this->AccessoryLightMulti::Move(0); // 000
		start = millis();
		while (millis() - start < 1000)
			this->ActionEnded();
	}
}

void SignalArduinoPattern::Move(int inDccId, byte inDccAccessory)
{
	int pospattern = 0;
	int etat = (inDccId - this->startingDcc) * 2 + inDccAccessory;

	// All leds off
	for (int led = 0; led < this->GetSize(); led++)
		this->SetState(led, LIGHTOFF);

	char symbText[16];
	for (int i = 0; i < 16-1; i++)
		symbText[i] = '-';

	symbText[15] = 0;

	// Change affected leds in the list
	for (int led = 0; led < PATTERN_NB_LEDS_MAX; led++)
	{
		byte c = pgm_read_byte(this->pPatterns + (etat * PATTERN_NB_LEDS_MAX) + pospattern++);

		if (c > 0 && c <= 100)
		{
			this->SetBlinking(c-1, 0);
			this->SetState(c-1, LIGHTON);
			symbText[c-1] = 'O';
		}
		else
			if (c > 100)
			{
				this->SetBlinking(c-101, this->blinkDuration);
				this->SetState(c-101, LIGHTBLINK);
				symbText[c-101] = 'B';
			}
	}

	Serial.println(symbText);
}

int SignalArduinoPattern::GetStatesNumber(const byte *pStates)
{
	int count = 0;
	for (int i = 0;; i++)
	{
		byte c = pgm_read_byte(pStates + i);
		if (c == PATTERN_END_LIST)
			return i;
	}

	return 99999;
}

//-------------------------------------------------------------------
// Un feu Français commun a au maximum 9 ampoules disposées comme suit:
//
//		9
//	6	8
//	5	7
//	4
//	3
//	2
//	1
//
// mais il y a plusieurs tailles intermédiaires:
//
//								
//							6	7
//				5			5
//				4			4
//	3			3			3
//	2			2			2
//	1			1			1
//
// voire même circulaire, avec deux ampoules supplémentaires (10 et 11)
//
//	6		7
//		3
//	10		11
//		1
//
// ou horizontal. Le terme horizontal est impropre, parce que ce feu peut être vertical, avec le 1 en haut. Malgré tout son 
// fonctionnement est le même quelque soit son orientation.
//
//	1	2
//
// Une douzième ampoule, dite oeilleton, est placée en bas à gauche du feu, à côté de la led 1.
//
// Le but de cette classe est de décrire correctement tous les types d'états réglementaires
// pour ces feux.
//
//-------------------------------------------------------------------

// La norme SCNF veut qu'il y ait quatre leds allumées au maximum, oeilleton compris.
// La liste décrit, 4 par 4, les numéros des leds allumées : positif allumé,
// supérieur à 100 clignotant, 0 inutilisé. Ce qui signifie que les numéros commencent à 1.
// Le code texte sur chaque ligne "--x---------" symbolise plus clairement les états.
// Enfin les numéros de chaque ligne sont notés de 0 à 15. Ils vont être réutilisés plus tard.

const byte SignalFrStates[] PROGMEM = {
						//  123456789012
	3, 0, 0, 0,			// "--x---------"   0: voie libre
	103, 0, 0, 0,		// "--B---------"   1: voie libre limitée à 160km/h	
	101, 0, 0, 0,		// "B-----------"   2: Avertissement arrêt très proche.	
	1, 0, 0, 0,			// "x-----------"   3: Avertissement arrêt.	
	102, 0, 0, 0,		// "-B----------"   4: Conduite à vue à 15Km/h maxi	
	2, 12, 0, 0,		// "-x---------x"   5: arrêt intégral, franchissable selon conditions	
	2, 5, 0, 0,			// "-x--x-------"   6: carré, arrêt intégral, non franchissable.	
	6, 7, 0, 0,			// "-----xx-----"   7: Vitesse réduite à 30	
	7, 9, 0, 0,			// "------x-x---"   8: Rappel de vitesse réduite à 30	
	1, 7, 9, 12,		// "x-----x-x--x"   9: Rappel de vitesse réduite à 30 + avertissement	
	101, 7, 9, 12,		// "B-----x-x--x"  10: Rappel de vitesse réduite à 30 + avertissement	
	106, 107, 0, 0,		// "-----BB-----"  11: Vitesse réduite à 60	
	101, 106, 107, 12,	// "B----BB----x"  12: Vitesse réduite à 60 + avertissement	
	107, 109, 0, 0,		// "------B-B---"  13: Rappel de vitesse réduite à 60	
	1, 107, 109, 12,	// "x-----B-B--x"  14: Rappel de vitesse réduite à 60 + avertissement	
	101, 107, 109, 12,	// "B-----B-B--x"  15: Rappel de vitesse réduite à 60 + avertissement
						//  123456789012
	PATTERN_END_LIST	// end of the list !
};

// Pour chaque type de feu, il n'y a que certains états de la liste ci-dessus qui sont utilisables.

const byte SignalFr3[] PROGMEM = { 0, 1, 2, 3, 4, PATTERN_END_LIST };
const byte SignalFr5[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, PATTERN_END_LIST };
const byte SignalFr7[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 11, 12, PATTERN_END_LIST };
const byte SignalFr9[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, PATTERN_END_LIST };

// Liste des états pour un feu rond.

const byte SignalFrStatesRound[] PROGMEM = {
	//  123456789012
	3, 0, 0, 0,	// "--x---------"	0: voie libre
	103, 0, 0, 0,	// "--B---------"	1: voie libre limitée à 160km/h
	101, 0, 0, 0,	// "B-----------"	2: Avertissement arrêt très proche.
	1, 0, 0, 0,	// "x-----------"	3: Avertissement arrêt.
	10, 11, 0, 0,	// "---------xx-"	4: Conduite à vue (signal rond seulement)
	6, 7, 0, 0,	// "-----xx-----"	5: Vitesse réduite à 30
	106, 107, 0, 0,	// "-----BB-----"	6: Vitesse réduite à 60
	101, 106, 107, 0,	// "B----BB-----"	7: Vitesse réduite à 60 + avertissement
						//  123456789012
	PATTERN_END_LIST	// end of the list !
};

// Tous les états sont utilisables, à priori.

const byte SignalFrRound[] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, PATTERN_END_LIST };

// Four leds on only. First led is 1. Negative led number will blink.

// Liste des états pour un feu horizontal.

const byte SignalFrStatesHorizontal[] PROGMEM = {
	//  123456789012
	1, 0, 0, 0,		// "x-----------" 	0: Vitesse de manoeuvre (feu horizontal)
	101, 0, 0, 0,	// "B-----------" 	1: Vitesse de manoeuvre réduite (feu horizontal)
	2, 0, 0, 0,		// "-x----------" 	2: idem AbsoluteStop mais sur un signal horizontal ou deux feux.
					//  123456789012
	PATTERN_END_LIST	// End of the list
};

// Tous les états sont utilisables, à priori.

const byte SignalFrHorizontal[] PROGMEM = { 0, 1, 2, PATTERN_END_LIST };

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            5

SignalArduinoPattern* signal;

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers

DriverArduino *arduino;

#define signalPattern	SignalFr7
#define NB_LEDS			12

int pins[NB_LEDS] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	Serial.begin(115200);
	UAD_StartSetup();

	// Le 'true' signifie que l'on va recevoir les codes dcc bruts. A voir si c'est gérable.
	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT, true);
	dccCommander.SetStatusLedPin(4);
	// Fonction qui sera appelée lors d'une réception de paquet accessoire.
	// mais en réalité, la gestion des groupes fait ça très bien !
	//dccCommander.SetBasicAccessoryDecoderPacketHandler(AccessoryDecoderPacket_Handler);

	int nb_etats = SignalArduinoPattern::GetStatesNumber(signalPattern);
	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(nb_etats)
		);

	// Ce petit bouton va permettre de passer en revue tous les codes dcc des feux en séquence...
	
	int dcc = 10;
	bool etat = false;
	for (int i = 0; i < nb_etats; i++)
	{
		if (!etat)
		{
			PUSH(buttonsCommander, 0)->AddDccId(dcc, 0);
			etat = true;
		}
		else
		{
			PUSH(buttonsCommander, 0)->AddDccId(dcc, 1);
			dcc++;
			etat = false;
		}
	}

//	PUSH(buttonsCommander, 0)->AddDccId(dcc, etat == true ? 1 : 0);

	PUSH(buttonsCommander, 0)->Setup(17);           // port A4

	accessories.Setup(1);

	arduino = new DriverArduino(NB_LEDS, 0);
	arduino->Setup();

	signal = new SignalArduinoPattern(arduino, NB_LEDS, pins, 0);
	signal->SetupSignal(10, 300, SignalFrStates, signalPattern);
	accessories.Add(signal);

	UAD_EndSetup();
}

void loop()
{

	if (dccCommander.Loop())
	{
		accessories.Loop();
		buttonsCommander.Loop();
		//signal->Loop();
	}
}
