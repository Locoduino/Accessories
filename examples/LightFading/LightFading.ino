/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Test for fading light with just one button>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

// total number of pushbuttons / accessories.
#define AccessoryNumber		2

#define LIGHT1			0
#define LIGHT2			1

// Accessories

Accessories accessories;
ButtonsCommander buttonsCommander;

// Drivers
	
DriverArduino *arduino;

//////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

    // One button
	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(2)
		);
    // This button will send commands to Dcc code 1/0 and 1/1, on pin 26
	PUSH(buttonsCommander, 0)->AddDccId(1, 0);
	PUSH(buttonsCommander, 0)->AddDccId(1, 1);
	PUSH(buttonsCommander, 0)->Setup(26);

	// Drivers setups

    // Two ports of the Arduino used by the leds, 10 and 11
	arduino = new DriverArduino(2, 0);
	arduino->Setup();
	arduino->SetupPortMotor(0, 10, ANALOG_INVERTED);
	arduino->SetupPortMotor(1, 11, ANALOG_INVERTED);

	// Accessories setups

    // Two lights in the accessory list.
	accessories.Setup(
		AccessoryNumber,
		new AccessoryLight(1, 0),
		new AccessoryLight(1, 1)
		);

    // Define fading/dimming effect
	LIGHT(accessories, LIGHT1)->SetFading(20, 20);
	LIGHT(accessories, LIGHT2)->SetFading(20, 20);

    // Attach each led to its Arduino port, with a given intensity.
	LIGHT(accessories, LIGHT1)->Setup(arduino, 0, 128);
	LIGHT(accessories, LIGHT2)->Setup(arduino, 1, 200);

    // Start with light2 on !
	LIGHT(accessories, LIGHT2)->LightOn();

	UAD_EndSetup();
}

void loop()
{
    // Only call UAD loops.
	accessories.Loop();
	buttonsCommander.Loop();
}