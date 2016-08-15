/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Demo for servo>
*************************************************************/

#include "Accessories.h"
#include "Commanders.h"

///////////////////////////////////////////////////////////////
// The target is to move a servo, allowing four buttons to change
// driven mode (fast, slow exclusive, memo and very slow).
// And four other buttons to reach four different positions.
// A classic button also toggle between min and max positions.

// Commanders

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard	action;
ButtonsCommanderKeyboard	pos1;
ButtonsCommanderKeyboard	pos2;
ButtonsCommanderKeyboard	pos3;
ButtonsCommanderKeyboard	pos4;
ButtonsCommanderKeyboard	mode1;
ButtonsCommanderKeyboard	mode2;
ButtonsCommanderKeyboard	mode3;
ButtonsCommanderKeyboard	mode4;
#else
ButtonsCommanderPush	action;
ButtonsCommanderPush	pos1;
ButtonsCommanderPush	pos2;
ButtonsCommanderPush	pos3;
ButtonsCommanderPush	pos4;
ButtonsCommanderPush	mode1;
ButtonsCommanderPush	mode2;
ButtonsCommanderPush	mode3;
ButtonsCommanderPush	mode4;
#endif

// Accessories

// Drivers

// We could use the arduino itself...	
DriverL293d driver;

// current user define speed.
int speed;

// servo accessory.
AccessoryServo servo;

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	switch (DCCID(inId))
	{
	case 101:	// Fast
		servo.SetDuration(0);
		Serial.println("Fast selected");
		break;
	case 102:	// Slow exclusive
		servo.SetDuration(5);
		Serial.println("Slow exclusive selected");
		break;
	case 103: // Slow with event memo
		servo.SetDuration(10);
		Serial.println("Slow selected");
		break;
	case 104: // Classic Slow. 
		servo.SetDuration(30);
		Serial.println("Classic slow selected");
		break;
	}

	Accessories::ReceiveEvent(inId, (ACCESSORIES_EVENT_TYPE)inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
	Commanders::SetEventHandler(ReceiveEvent);
	Commanders::SetStatusLedPin(LED_BUILTIN);

#ifdef VISUALSTUDIO
	action.begin(DCCINT(22, 0), '0');
	pos1.begin(DCCINT(30, 0), '1');
	pos2.begin(DCCINT(30, 1), '2');
	pos3.begin(DCCINT(31, 0), '3');
	pos4.begin(DCCINT(31, 1), '4');

	mode1.begin(DCCINT(101, 0), 'a');
	mode2.begin(DCCINT(102, 0), 'z');
	mode3.begin(DCCINT(103, 0), 'e');
	mode4.begin(DCCINT(104, 0), 'r');
#else
	action.begin(DCCINT(22, 0), 40');
	pos1.begin(DCCINT(30, 0), 30);
	pos2.begin(DCCINT(30, 1), 31);
	pos3.begin(DCCINT(31, 0), 32');
	pos4.begin(DCCINT(31, 1), 33');

	mode1.begin(DCCINT(101, 0), 40);
	mode2.begin(DCCINT(102, 0), 41);
	mode3.begin(DCCINT(103, 0), 42);
	mode4.begin(DCCINT(104, 0), 43);
#endif

	driver.begin();

	DriverPort *pPort = driver.beginPortServo(L293D_PORT_SERVO1);

	speed = 19;	// starting speed state : fast.

	// Accessories setups

	servo.begin(pPort, 0, 40, 50, 4);
	servo.AddMovingPosition(DCCINT(30, 0), 10);
	servo.AddMovingPosition(DCCINT(30, 1), 20);
	servo.AddMovingPosition(DCCINT(31, 0), 30);
	servo.AddMovingPosition(DCCINT(31, 1), 40);
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}
