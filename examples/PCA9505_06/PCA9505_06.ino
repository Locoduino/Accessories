/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Sample for PCA9505_06 port expander usage>
*************************************************************/

#include "Accessories.h"

AccessoryLight light0, light1, light2;

// List of the ports on the Arduino. Light Ports are handled in analog mode for fading.
PortExpanderPCA9505_06 expander;

#define EXPANDER_ID	10

PortOnePin PortLight0;
PortOnePin PortLight1;
PortOnePin PortLight2;

//////////////////////////////////
//
// Setup
//
void setup()
{
	Serial.begin(115200);
//	while (!Serial);		// For Leonardo only. No effect on other Arduino.

	Accessories::begin();

	// Ports setups

	expander.begin(EXPANDER_ID, 0x27);

	PortLight0.begin(EXPANDER_PIN(EXPANDER_ID, 0), DIGITAL);
	PortLight1.begin(EXPANDER_PIN(EXPANDER_ID, 4), DIGITAL);
	PortLight2.begin(EXPANDER_PIN(EXPANDER_ID, 7), DIGITAL);
	
	// Accessories setups

    // Declare accessories.
	light0.begin(&PortLight0, 1, 1000);
	light1.begin(&PortLight1, 2, 550);
	light2.begin(&PortLight2, 3, 1500);
	
	light0.Blink();
	light1.Blink();
	light2.Blink();
}

void loop()
{
	Accessories::loop();
}
