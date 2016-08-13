/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Test for UAD library, with one light, controled by Dcc.>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT          5

// Accessories

Accessories accessories;
DccCommander dccCommander;

// Drivers
	
DriverArduino *arduino;

//////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

    // Setup of the Dcc commander.
	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dccCommander.SetStatusLedPin(13);

	// Drivers setups

    // one light is connected to the arduino.
	arduino = new DriverArduino(1, 0); // 1 motor/led, 0 servos
	arduino->Setup();
	arduino->SetupPortMotor(0, 12); // first port (0), pin 12
	
	// Accessories setups

    // Assign Dcc code for each accessory.
	accessories.Setup(1);
	accessories.Add(new AccessoryLight(1, 0)); // dcc 1/0

   // Attach each accessory to its driver/port.
	LIGHT(accessories, 0)->Setup(arduino, 0); // use port 0 of arduino driver

	UAD_EndSetup();
}

void loop()
{
    // Dcc is run first, and if necessary, lets the accessories work.
	if (dccCommander.Loop())
	{
		accessories.Loop();
	}
}