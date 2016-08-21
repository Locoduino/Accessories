/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Test for Accessories library, with some turnouts and lights, controled by Dcc.>
*************************************************************/

#include "Commanders.h"
#include "Accessories.h"

#define IDLEFT		DCCINT(20, 0)
#define IDRIGHT		DCCINT(20, 1)
#define IDDC		DCCINT(21, 0)
#define IDEPS		DCCINT(21, 1)
#define IDTJD		DCCINT(22, 0)
#define IDSERVOMIN	DCCINT(22, 1)
#define IDSERVOMAX	DCCINT(23, 0)
#define IDLIGHT		DCCINT(23, 1)

// Commanders

SERIAL_COMMANDER(Serial);

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard push0;
ButtonsCommanderKeyboard push1;
ButtonsCommanderKeyboard push2;
ButtonsCommanderKeyboard push3;
ButtonsCommanderKeyboard push4;
ButtonsCommanderKeyboard push5;
ButtonsCommanderKeyboard push6;
ButtonsCommanderKeyboard push7;
#else
ButtonsCommanderPush push0;
ButtonsCommanderPush push1;
ButtonsCommanderPush push2;
ButtonsCommanderPush push3;
ButtonsCommanderPush push4;
ButtonsCommanderPush push5;
ButtonsCommanderPush push6;
ButtonsCommanderPush push7;
#endif

// Accessories

AccessoryMotorTwoWays turnoutLeft;
AccessoryMotorTwoWays turnoutRight;
AccessoryMotorTwoWays turnoutDc;
AccessoryMotorTwoWays turnoutEps;
AccessoryMotorTwoWays turnoutTjd;
AccessoryServo servo;
AccessoryLight light;

// Drivers

DriverL293d l293d;
DriverL298n l298n;

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
	// Setup of the Dcc commander.
	SerialCommander.begin(115200);
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
	Commanders::SetEventHandler(ReceiveEvent);
	Commanders::SetStatusLedPin(LED_BUILTIN);

	// Each button assigned to an accessory Dcc code.
#ifdef VISUALSTUDIO
	push0.begin(IDLEFT, '0');
	push1.begin(IDRIGHT, '1');
	push2.begin(IDDC, '2');
	push3.begin(IDEPS, '3');
	push4.begin(IDTJD, '4');
	push5.begin(IDSERVOMIN, '5');
	push6.begin(IDSERVOMAX, '6');
	push7.begin(IDLIGHT, '7');
#else
	push0.begin(IDLEFT, 30);
	push1.begin(IDRIGHT, 32);
	push2.begin(IDDC, 34);
	push3.begin(IDEPS, 36);
	push4.begin(IDTJD, 38);
	push5.begin(IDSERVOMIN, 40);
	push6.begin(IDSERVOMAX, 42);
	push7.begin(IDLIGHT, 44);
#endif
	// Drivers setups

	// four turnouts are connected to the l293d shield.
	l293d.begin();
	DriverPort *pPortLeft = l293d.beginPortMotor(L293D_PORT_M1, MOTOR12_1KHZ);
	DriverPort *pPortRight = l293d.beginPortMotor(L293D_PORT_M2, MOTOR12_1KHZ);
	DriverPort *pPortDc = l293d.beginPortMotor(L293D_PORT_M3, MOTOR34_1KHZ);
	DriverPort *pPortEps = l293d.beginPortMotor(L293D_PORT_M4, MOTOR34_1KHZ);
	DriverPort *pPortServo = l293d.beginPortServo(L293D_PORT_SERVO1);

	// The double turnout is on the l298n circuit, with the led.
	l298n.begin();
	DriverPort *pPortTjd = l298n.beginPortMotor(L298N_PORT_OUT1, 50, 52);
	DriverPort *pPortLight = l298n.beginPortMotor(L298N_PORT_OUT2, 46, 48);

	// Accessories setups

	// Assign Dcc code for each accessory.
	turnoutLeft.begin(pPortLeft, IDLEFT, 50, 150);
	turnoutRight.begin(pPortRight, IDRIGHT, 3000, 150);
	turnoutDc.begin(pPortDc, IDDC, 500, 150);
	turnoutEps.begin(pPortEps, IDEPS, 3000, 150);

	turnoutTjd.begin(pPortTjd, IDTJD, 0, 150);
	light.begin(pPortLight, IDLIGHT, 500, 150);
	servo.begin(pPortServo, 1000, 40, 50, 2);
	servo.AddMinMaxMovingPositions(IDSERVOMIN, IDSERVOMAX);
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}