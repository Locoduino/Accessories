/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Sample for four 3 leds signals on a Uno>
*************************************************************/
#include "UniversalAccessoryDecoder.h"

//------------------------------------------------------------------------------
// SignalArduino declaration

class SignalArduino : public AccessoryLightMulti
{
public:
	SignalArduino(DriverArduino *inpDriver, byte inNbLeds, int *inpPins, int inFirstPort = 0);
	void SetupSignal(int inStartingDcc);
};

//------------------------------------------------------------------------------
// SignalArduino definition

SignalArduino::SignalArduino(DriverArduino *inpDriver, byte inNbLeds, int *inpPins, int inFirstPort) : AccessoryLightMulti(0, 0, inNbLeds, 0)
{
	for (int led = 0; led < inNbLeds; led++)
	{
		inpDriver->SetupPortMotor(inFirstPort + led, inpPins[led], DIGITAL);
	}

	for (int led = 0; led < inNbLeds; led++)
	{
		// Led number is also port number...
		this->SetupLight(led, inpDriver, inFirstPort + led, 255);
	}
}

void SignalArduino::SetupSignal(int inStartingDcc)
{
	this->Setup();

	int led_on = 0;

	// Used dcc codes are
	//                 Led  0    1    2
	// inStartingDcc   / 0  on  off  off
	// inStartingDcc   / 1  off on   off
	// inStartingDcc+1 / 0  off off  on
	// inStartingDcc+1 / 1  off off  off

	this->AdjustDccPositionsSize(this->GetSize()+1);

	int dcc = inStartingDcc;
	bool etat = false;
	for (int i = 0; i < this->GetSize(); i++)
	{
		if (!etat)
		{
			this->AddDccPosition(dcc, 0, 1<<i);
			etat = true;
		}
		else
		{
			this->AddDccPosition(dcc, 1, 1<<i);
			dcc++;
			etat = false;
		}
	}
	
	this->AddDccPosition(dcc, etat==true?1:0, 0);

	// Light on at startup

	this->Move(7); // 111
	unsigned long start = millis();
	while(millis() - start < 1000)
		this->ActionEnded();
	
	this->Move(0); // 000
	start = millis();
	while(millis() - start < 1000)
		this->ActionEnded();
}

//------------------------------------------------------------------------------
// Classic INO area

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            5

#define NB_LEDS		3
#define NB_ETATS	4
#define NB_FEUX		4

SignalArduino* signaux[NB_FEUX];

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers

DriverArduino *arduino;

int pins[NB_FEUX][NB_LEDS] = {
	{ 2, 3, 4 },
	{ 5, 6, 7 },
	{ 8, 9, 10 },
	{ 11, 12, 13 },
};

int dcc_codes[NB_FEUX] = { 10, 12, 14, 16 };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dccCommander.SetStatusLedPin(18);

	// Small push button to check all signal states manually.
	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(NB_FEUX * NB_ETATS)
		);

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		int dcc = dcc_codes[feu];
		bool etat = false;
		for (int i = 0; i < NB_LEDS; i++)
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
		
		PUSH(buttonsCommander, 0)->AddDccId(dcc, etat==true?1:0);
	}

	PUSH(buttonsCommander, 0)->Setup(17);           // port A4

													// NB_LEDS * NB_FEUX ports to initialize
	arduino = new DriverArduino(NB_LEDS * NB_FEUX, 0);
	arduino->Setup();

	accessories.Setup(NB_FEUX);

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		signaux[feu] = new SignalArduino(arduino, NB_LEDS, pins[feu], feu * NB_LEDS);
		signaux[feu]->SetupSignal(dcc_codes[feu]);

		accessories.Add(signaux[feu]);
	}

	UAD_EndSetup();
}

void loop()
{
	if (dccCommander.Loop())
	{
		accessories.Loop();
		buttonsCommander.Loop();
	}
}
