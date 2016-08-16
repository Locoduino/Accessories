/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Test for fading light with just one button>
*************************************************************/

#include "Commanders.h"
#include "Accessories.h"

// Commanders

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard	push;
#else
ButtonsCommanderPush push;
#endif

// Accessories

AccessoryLight red, green;

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

	// One button
    // This button will send commands to Dcc code 1/0 and 1/1, on pin 26
#ifdef VISUALSTUDIO
	push.begin(DCCINT(1, 0), '0');
#else
	push.begin(DCCINT(1, 0), 26);
#endif
	push.AddEvent(DCCINT(1, 1));

	// Drivers setups

    // Two ports of the Arduino used by the leds, 10 and 11
	arduino.begin();
	DriverPort *pPort0 = arduino.AddPortMotor(10, ANALOG_INVERTED);
	DriverPort *pPort1 = arduino.AddPortMotor(11, ANALOG_INVERTED);

	// Accessories setups

    // Two lights in the accessory list.

	red.begin(pPort0, DCCINT(1, 0), 0, 120);
	green.begin(pPort1, DCCINT(1, 1), 0, 200);

    // Define fading/dimming effect
	red.SetFading(20, 20);
	green.SetFading(20, 20);

    // Start with light0 on !
	red.LightOn();
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}