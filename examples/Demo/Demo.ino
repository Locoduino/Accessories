/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Test for Accessories library, with some turnouts and lights, controled by Dcc.>
*************************************************************/

#include "Commanders.h"
#include "Accessories.h"

// Commanders

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
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
	Commanders::SetEventHandler(ReceiveEvent);
	Commanders::SetStatusLedPin(LED_BUILTIN);

	// Each button assigned to an accessory Dcc code.
#ifdef VISUALSTUDIO
	push0.begin(DCCINT(20, 0), '0');
	push1.begin(DCCINT(20, 1), '1');
	push2.begin(DCCINT(21, 0), '2');
	push3.begin(DCCINT(21, 1), '3');
	push4.begin(DCCINT(22, 0), '4');
	push5.begin(DCCINT(22, 1), '5');
	push6.begin(DCCINT(23, 0), '6');
	push7.begin(DCCINT(23, 1), '7');
#else
	push0.begin(DCCINT(20, 0), 30);
	push1.begin(DCCINT(20, 1), 32);
	push2.begin(DCCINT(21, 0), 34);
	push3.begin(DCCINT(21, 1), 36);
	push4.begin(DCCINT(22, 0), 38);
	push5.begin(DCCINT(22, 1), 40);
	push6.begin(DCCINT(23, 0), 42);
	push7.begin(DCCINT(23, 1), 44);
#endif
	// Drivers setups

	// four turnouts are connected to the l293d shield.
	l293d.begin();
	DriverPortL293d *pPortLeft = l293d.beginPortMotor(L293D_PORT_M1, MOTOR12_1KHZ);	//TURNOUT_LEFT				  
	DriverPortL293d *pPortRight = l293d.beginPortMotor(L293D_PORT_M2, MOTOR12_1KHZ);	//TURNOUT_RIGHT
	DriverPortL293d *pPortDc = l293d.beginPortMotor(L293D_PORT_M3, MOTOR34_1KHZ);	//TURNOUT_DC
	DriverPortL293d *pPortEps = l293d.beginPortMotor(L293D_PORT_M4, MOTOR34_1KHZ);		//TURNOUT_EPS
	DriverPortServoArduino *pPortServo = l293d.beginPortServo(L293D_PORT_SERVO1);		//SERVO

																						// one is on the l298n circuit, with the led.										
	l298n.begin();
	DriverPortL298n *pPortTjd = l298n.beginPortMotor(L298N_PORT_OUT1, 50, 52);		//TURNOUT_TJD								 
	DriverPortL298n *pPortLight = l298n.beginPortMotor(L298N_PORT_OUT2, 46, 48);		//LIGHT1

																						// Accessories setups

																						// Assign Dcc code for each accessory.
	turnoutLeft.begin(pPortLeft, DCCINT(20, 0), 50., 150);
	turnoutRight.begin(pPortRight, DCCINT(20, 1), 3000, 150);
	turnoutDc.begin(pPortDc, DCCINT(21, 0), 500, 150);
	turnoutEps.begin(pPortEps, DCCINT(21, 1), 3000, 150);

	turnoutTjd.begin(pPortTjd, DCCINT(22, 0), 0, 150);
	light.begin(pPortLight, DCCINT(21, 1), 500, 150);	// LIGHT1
	servo.begin(pPortServo, 1000, 40, 50, 2);

	servo.AddMinMaxMovingPositions(DCCINT(23, 0), DCCINT(23, 1));
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}