/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <2 servos and 3 leds driven by dcc arduino>
*************************************************************/

#include <Accessories.h>

#include <Commanders.h>

#define kDCC_INTERRUPT      0

ButtonsCommanderPush push;
ButtonsCommanderKeyboard keyb0, keyb1, keyb2, keyb4, keyb5;
ButtonsCommanderSwitch swiitch;	// two 'ii' to avoid collision with the C word 'switch' !
ButtonsCommanderPotentiometer potar; //(321, 0, 20, 145)	// Link it to SERVO1, from 20 to 145 degrees

SERIAL_COMMANDER(Serial);

// Accessories

AccessoryServo servo1;
AccessoryServo servo2;
AccessoryLight green;
AccessoryLight orange;
AccessoryLight white;

AccessoryGroup groupServos;
AccessoryGroup groupLights;

// Drivers

DriverArduino arduino;

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
	Commanders::SetEventHandler(ReceiveEvent);
	Commanders::SetStatusLedPin(LED_BUILTIN);

	// Setup of Dcc commander
	DccCommander.begin(0x00, 0x00, kDCC_INTERRUPT);
	SerialCommander.begin(115200);

	keyb0.begin(1000, '0');
	keyb1.begin(DCCINT(319, 0), '1');
	keyb2.begin(DCCINT(319, 1), '2');

	keyb4.begin(DCCINT(320, 0), '4');
	keyb5.begin(DCCINT(320, 1), '5');

	// The push button send two Dcc codes in turn, and is connected to pin 26.
	push.begin(1000, 26);	// id 1000, pin 26
	push.AddEvent(DCCINT(320, 0));
	push.AddEvent(DCCINT(320, 1));

	// The switch have two positions, each one controls one Dcc code. Each one connected to its pin.
	swiitch.begin();
	swiitch.AddEvent(24, DCCINT(319, 0));
	swiitch.AddEvent(25, DCCINT(319, 1));

	// The potentiometer is on analog pin 8.
	potar.begin(8, 1001, 0, 100);

	// Drivers setups

	// List of the ports on the Arduino. Pors 9,10 and 11 are handled in analog mode for fading.
	arduino.begin();
	DriverPort *pPortServo1 = arduino.AddPortServo(2);
	DriverPort *pPortServo2 = arduino.AddPortServo(3);
	DriverPort *pPortLight1 = arduino.AddPortMotor(9, ANALOG);
	DriverPort *pPortLight2 = arduino.AddPortMotor(10, ANALOG);
	DriverPort *pPortLight3 = arduino.AddPortMotor(11, ANALOG);

	// Accessories setups

	// Attach the servos to their driver/ports.
	// Servo1 can move from 30 to 145 degrees.
	servo1.begin(pPortServo1, 20, 30, 45);
	servo1.AddMovingPosition(DCCINT(316, 0), MINIMUM);
	servo1.AddMovingPosition(DCCINT(316, 1), MAXIMUM);

	// Servo1 has a pin 49 to control a relay giving power to the servo.
	servo1.SetPowerCommand(49);

	// Servo2 can move from 10 to 150 degrees. But there is also two more positions at 45 and 135 degrees,
	// commanded by Dcc codes 315/0 and 315/1.
	servo2.begin(pPortServo2, 0, 45, 55, 2);
	servo2.AddMovingPosition(DCCINT(314, 0), MINIMUM);
	servo2.AddMovingPosition(DCCINT(314, 1), MAXIMUM);

	// Attach the lights to their driver/ports.
	green.begin(pPortLight1, DCCINT(1, 0));
	orange.begin(pPortLight2, DCCINT(1, 1));
	white.begin(pPortLight3, DCCINT(2, 0));

	// Declare light fading/dimming.
	green.SetFading(20, 10);
	orange.SetFading(20, 10);
	//light3.SetFading(20, 10);

	groupLights.AddState(DCCINT(319, 0));
	groupLights.AddStateItem(DCCINT(319, 0), green, LIGHTON);
	groupLights.AddStateItem(DCCINT(319, 0), orange, LIGHTOFF);
	groupLights.AddStateItem(DCCINT(319, 0), white, LIGHTON);

	groupLights.AddState(DCCINT(319, 1));
	groupLights.AddStateItem(DCCINT(319, 1), green, LIGHTOFF);
	groupLights.AddStateItem(DCCINT(319, 1), orange, LIGHTON);
	groupLights.AddStateItem(DCCINT(319, 1), white, LIGHTOFF);

	groupServos.AddState(DCCINT(320, 0), true);
	groupServos.AddStateItem(DCCINT(320, 0), servo1, MINIMUM, 500);
	groupServos.AddStateItem(DCCINT(320, 0), servo2, MINIMUM, 500);

	groupServos.AddState(DCCINT(320, 1), true);
	groupServos.AddStateItem(DCCINT(320, 1), servo1, MAXIMUM, 500);
	groupServos.AddStateItem(DCCINT(320, 1), servo2, MAXIMUM, 500);
}

void loop()
{
	Commanders::loop();
	Accessories::loop();
}
