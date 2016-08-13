/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Test for UAD library, with five leds, controled by Dcc.>
*************************************************************/

#include "UniversalAccessoryDecoder.h"
#include "binary.h"

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
	arduino = new DriverArduino(5, 0); // 1 motor/led, 0 servos
	arduino->Setup();
	arduino->SetupPortMotor(0, 4); // first port (0), pin 4
	arduino->SetupPortMotor(1, 5); // first port (1), pin 5
	arduino->SetupPortMotor(2, 6); // first port (2), pin 6
	arduino->SetupPortMotor(3, 7); // first port (3), pin 7
	arduino->SetupPortMotor(4, 8); // first port (4), pin 8

	// Accessories setups

	AccessoryLightMulti *pSignal = new AccessoryLightMulti(0, 0, 5, 1000); // dcc not significant here

	// Assign Dcc code for each accessory.
	accessories.Setup(1);
	accessories.Add(pSignal);

	// Attach each light to its driver/port.
	pSignal->Setup();
	pSignal->SetupLight(0, arduino, 0); // use port 0 of arduino driver
	pSignal->SetupLight(1, arduino, 1); // use port 0 of arduino driver
	pSignal->SetupLight(2, arduino, 2); // use port 0 of arduino driver
	pSignal->SetupLight(3, arduino, 3); // use port 0 of arduino driver
	pSignal->SetupLight(4, arduino, 4); // use port 0 of arduino driver

	pSignal->AdjustDccPositionsSize(10);

	pSignal->AddDccPosition(20, 0, B10000, B10000);
	pSignal->AddDccPosition(21, 0, B01000, B01000);
	pSignal->AddDccPosition(22, 0, B00100, B00100);
	pSignal->AddDccPosition(23, 0, B00010, B00010);
	pSignal->AddDccPosition(24, 0, B00001, B00001);
	pSignal->AddDccPosition(25, 0, B10001, B10000);
	pSignal->AddDccPosition(26, 0, B01001, B01000);
	pSignal->AddDccPosition(27, 0, B00101, B00100);
	pSignal->AddDccPosition(28, 0, B00011, B00010);
	pSignal->AddDccPosition(29, 0, B00101, B00001);

	UAD_EndSetup();

	// Test code to pass through all the states during 5 seconds...
	for (int i = 0; i < pSignal->GetDccPositionSize(); i++)
	{
		pSignal->Move(20 + i, 0);

		unsigned long start = millis();

		while (millis() - start < 5000)
		{
			loop();
		}
	}
}

void loop()
{
	// Dcc is run first, and if necessary, lets the accessories work.
	if (dccCommander.Loop())
	{
		accessories.Loop();
	}
}