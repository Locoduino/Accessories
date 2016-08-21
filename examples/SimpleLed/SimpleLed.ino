/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Test for Accessories library, with one light, controled by Dcc.>
*************************************************************/

#include "Commanders.h"
#include "Accessories.h"

// DCC codes
#define ACTION		DCCINT(20,0)

// Commanders

SERIAL_COMMANDER(Serial)

// Accessories

AccessoryLight  light;

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
	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));
	SerialCommander.begin(115200);

	// Drivers setups

    // one light is connected to the arduino.
	arduino.begin();
	DriverPort *pPort = arduino.AddPortMotor(12);
	
	// Accessories setups

    // Assign Dcc code for each accessory.
	light.begin(pPort, ACTION);
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}