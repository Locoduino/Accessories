#include "Commanders.h"
#include "Accessories.h"

// Accessories
AccessoryMotorTwoWays aiguille;

ButtonsCommanderPush push;

SERIAL_COMMANDER(Serial);

// Drivers
PortTwoPins port;

#define DCCID_DROIT   DCCINT(20, 0)
#define DCCID_DEVIE   DCCINT(20, 1)

void setup()
{
	Serial.begin(115200);
	Commanders::begin(LED_BUILTIN);
	Accessories::begin();

	SerialCommander.begin();

	// Setup of the Dcc commander.
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(2), true);

	// Setup of the buttons, one by accessory
	push.begin(DCCID_DROIT, 4);
	push.AddEvent(DCCID_DEVIE);

	// Setup of ports
	port.begin(6, 7, DIGITAL);

	// Accessories setups

	aiguille.beginTwoWays(&port, DCCID_DROIT, DCCID_DEVIE, 255, 250);
}

void loop()
{
	unsigned long id = Commanders::loop();

	if (id != UNDEFINED_ID)
	{
		// Renvoie l'événement reçu de Commanders, vers les accessoires...
		Accessories::RaiseEvent(id, (ACCESSORIES_EVENT_TYPE)Commanders::GetLastEventType(), Commanders::GetLastEventData());
	}

	Accessories::loop();
}

#if 0
/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <2 servos and 3 leds driven by dcc arduino>
*************************************************************/

#include <Accessories.h>
#include <Commanders.h>

ButtonsCommanderPush push;
ButtonsCommanderKeyboard greenButton, orangeButton, whiteButton;
ButtonsCommanderKeyboard servo1Button;
ButtonsCommanderKeyboard servo2_minButton, servo2_maxButton;
ButtonsCommanderKeyboard motorOneWayButton;
ButtonsCommanderKeyboard motorTwoWaysButton;
ButtonsCommanderKeyboard groupServos1Button;
ButtonsCommanderKeyboard groupServos2Button;
ButtonsCommanderKeyboard groupLights1Button;
ButtonsCommanderKeyboard groupLights2Button;
ButtonsCommanderSwitch swiitch;	// two 'ii' to avoid collision with the C word 'switch' !
ButtonsCommanderPotentiometer potar;

SERIAL_COMMANDER(Serial);

// Accessories

AccessoryServo servo1;
AccessoryServo servo2;
AccessoryLight green;
AccessoryLight orange;
AccessoryLight white;

AccessoryMotorOneWay motorOneWay;
AccessoryMotorTwoWays motorTwoWays;

AccessoryGroup groupServos;
AccessoryGroup groupLights;

// Ports

PortServo PortServo1;
PortServo PortServo2;

PortOnePin PortLightGreen;
PortOnePin PortLightOrange;
PortOnePin PortLightWhite;

PortOnePin PortMotorOneWay;
PortTwoPins PortMotorTwoWays;

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Accessories::ReceiveEvent(inId, (ACCESSORIES_EVENT_TYPE)inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::begin(ReceiveEvent, LED_BUILTIN);
	DccCommander.begin(0, 0, 2);

	Accessories::begin(10, 1000);

	Serial.begin(115200);
	SerialCommander.begin();

	greenButton.begin(1000, '1');
	orangeButton.begin(1001, '2');
	whiteButton.begin(1002, '3');

	servo1Button.begin(2000, '0');

	servo2_minButton.begin(2100, '-');
	servo2_maxButton.begin(2101, '+');

	motorOneWayButton.begin(3000, '6');

	motorTwoWaysButton.begin(3100, '9');
	
	groupServos1Button.begin(5000, '4');
	groupServos2Button.begin(5001, '5');

	groupLights1Button.begin(5100, '7');
	groupLights2Button.begin(5101, '8');

	// The push button send two Dcc codes in turn, and is connected to pin 26.
	push.begin(1000, 26);	// id 1000, pin 26
	push.AddEvent(DCCINT(320, 0));
	push.AddEvent(DCCINT(320, 1));

	// The switch have two positions, each one controls one Dcc code. Each one connected to its pin.
	swiitch.begin();
	swiitch.AddEvent(DCCINT(319, 0), 24);
	swiitch.AddEvent(DCCINT(319, 1), 25);

	// The potentiometer is on analog pin 8.
	potar.begin(1001, 8, 0, 100);

	// Drivers setups

	// List of the ports on the Arduino. Pors 9,10 and 11 are handled in analog mode for fading.
	PortServo1.begin(2);
	PortServo2.begin(3);

	PortLightGreen.begin(9, ANALOG);
	PortLightOrange.begin(10, ANALOG);
	PortLightWhite.begin(11, ANALOG);

	PortMotorOneWay.begin(15, ANALOG);
	PortMotorTwoWays.begin(20, 21, ANALOG_INVERTED);

	// Accessories setups

	// Attach the servos to their driver/ports.
	// Servo1 can move from 30 to 145 degrees.
	servo1.begin(&PortServo1, 20, 30, 45);
	servo1.AddMovingPosition(2000, MINIMUM);
	servo1.AddMovingPosition(2001, MAXIMUM);

	// Servo1 has a pin 49 to control a relay giving power to the servo.
	servo1.SetPowerCommand(49);

	// Servo2 can move from 10 to 150 degrees. But there is also two more positions at 45 and 135 degrees,
	// commanded by Dcc codes 315/0 and 315/1.
	servo2.begin(&PortServo2, 0, 45, 55, 2);
	servo2.AddMovingPosition(2100, MINIMUM);
	servo2.AddMovingPosition(2101, MAXIMUM);

	// Attach the lights to their driver/ports.
	green.begin(&PortLightGreen, 1000);
	orange.begin(&PortLightOrange, 1001);
	white.begin(&PortLightWhite, 1002);

	// Declare light fading/dimming.
	green.SetFading(20, 10);
	orange.SetFading(20, 10);
	//white.SetFading(20, 10);

	motorOneWay.begin(&PortMotorOneWay, 3000, 200, 200);
	motorTwoWays.begin(&PortMotorTwoWays, 3100, 200, 200);

	groupLights.AddState(5100);
	groupLights.AddStateItem(5100, green, LIGHTON);
	groupLights.AddStateItem(5100, orange, LIGHTOFF);
	groupLights.AddStateItem(5100, white, LIGHTON);

	groupLights.AddState(5101);
	groupLights.AddStateItem(5101, green, LIGHTOFF);
	groupLights.AddStateItem(5101, orange, LIGHTON);
	groupLights.AddStateItem(5101, white, LIGHTOFF);

	groupServos.AddState(5000, true);
	groupServos.AddStateItem(5000, servo1, MINIMUM, 500);
	groupServos.AddStateItem(5000, servo2, MINIMUM, 500);

	groupServos.AddState(5001, true);
	groupServos.AddStateItem(5001, servo1, MAXIMUM, 500);
	groupServos.AddStateItem(5001, servo2, MAXIMUM, 500);

	Commanders::printCommanders();
}

void loop()
{
	Commanders::loop();
	Accessories::loop();
}
#endif

#if 0
#include <Accessories.h>
#include <Commanders.h>

// Les boutons...
ButtonsCommanderPush boutonDecoupleur;
ButtonsCommanderSwitch boutonAiguillageGauche;
ButtonsCommanderSwitch boutonAiguillageDroite;

// Les trois moteurs
AccessoryMotorTwoWays aiguillageGauche;
AccessoryMotorTwoWays aiguillageDroite;
AccessoryMotorOneWay decouplage;

// Les ports pour connecter les moteurs.
PortTwoPins portAiguillageGauche;
PortTwoPins portAiguillageDroite;
PortOnePin portDecoupleur;

SERIAL_COMMANDER(Serial);

void setup()
{
	Serial.begin(115200);

	Commanders::begin();
	Accessories::begin();

	// Activation de la reception de messages Dcc
	// Ne fera rien en analogique... Mais on peut carrement enlever la ligne si besoin de mémoire.
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
	SerialCommander.begin();

	// Gestion des boutons pour le digital ou le numérique.
	// Ne fera rien si aucun bouton n'est branché, mais on peut enlever si besoin de mémoire.
	//
	// Les boutons doivent décrire quel identifiant est concerné s'ils sont activés.
	// Un switch peut avoir une, deux ou beaucoup plus de positions.
	// Ici, seules deux positions sont déclarées avec pour chacune une broche associée, et un identifiant émis.
	boutonAiguillageGauche.begin();
	boutonAiguillageGauche.AddEvent(DCCINT(15, 0), 2);
	boutonAiguillageGauche.AddEvent(DCCINT(15, 1), 7);

	boutonAiguillageDroite.begin();
	boutonAiguillageDroite.AddEvent(DCCINT(16, 0), 4);
	boutonAiguillageDroite.AddEvent(DCCINT(16, 1), 5);

	boutonDecoupleur.begin(DCCINT(17, 0), 6);

	// Les ports avec leurs broches en digital (pas PWM)
	portAiguillageGauche.begin(8, 9, DIGITAL);
	portAiguillageGauche.begin(10, 11, DIGITAL);
	portDecoupleur.begin(12, DIGITAL);

	// Les accessoires avec pour chaque, le port utilisé et le ou les identifiants associés à chaque position. 
	// 255 est la vitesse, ici au maxi, et 400 est la durée d'activation du moteur en millisecondes.
	// C'est encore un peu long pour des solénoïdes. Il faudrait adapter selon le modèle de moteur...
	aiguillageGauche.beginTwoWays(&portAiguillageGauche, DCCINT(15, 0), DCCINT(15, 1), 255, 400);
	aiguillageDroite.beginTwoWays(&portAiguillageDroite, DCCINT(16, 0), DCCINT(16, 1), 255, 400);
	decouplage.begin(&portDecoupleur, DCCINT(17, 0), 255, 400);
}

void loop()
{
	unsigned long id = Commanders::loop();

	if (id != UNDEFINED_ID)
	{
		// Renvoie l'événement reçu de Commanders, vers les accessoires...
		Accessories::RaiseEvent(id, (ACCESSORIES_EVENT_TYPE) Commanders::GetLastEventType(), Commanders::GetLastEventData());
	}

	Accessories::loop();
}
#endif

#if 0
#include <Commanders.h>

void setup()
{

	Serial.begin(115200);

	Commanders::begin(LED_BUILTIN);
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
}

void loop()
{
	unsigned long ret = Commanders::loop();

	if (ret != UNDEFINED_ID)
	{
		Serial.print(DCCID(ret));
		Serial.print("   ");
		Serial.print(DCCACTIVATION(ret));
		Serial.println("     ");
	}
}
#endif