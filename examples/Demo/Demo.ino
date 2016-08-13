/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Test for UAD library, with some turnouts and lights, controled by Dcc.>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT          5

// total number of pushbuttons / accessories.
#define AccessoryNumber		7

#define TURNOUT_LEFT		0
#define TURNOUT_DC		1
#define TURNOUT_RIGHT		2
#define TURNOUT_EPS		3
#define TURNOUT_TJD		4
#define SERVO1			5
#define LIGHT1			6

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers
	
DriverL293d *l293d;
DriverL298n *l298n;

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

    // Setup of the buttons, one by accessory
	buttonsCommander.Setup(7,
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1),
		new ButtonsCommanderPush(1)
		);
    // Each button assigned to an accessory Dcc code.
	PUSH(buttonsCommander, 0)->AddDccId(20, 0);
	PUSH(buttonsCommander, 1)->AddDccId(20, 1);
	PUSH(buttonsCommander, 2)->AddDccId(21, 0);
	PUSH(buttonsCommander, 3)->AddDccId(21, 1);
	PUSH(buttonsCommander, 4)->AddDccId(22, 0);
	PUSH(buttonsCommander, 5)->AddDccId(22, 1);
	PUSH(buttonsCommander, 6)->AddDccId(23, 0);

    // Declare the Arduino pins.
	PUSH(buttonsCommander, 0)->Setup(30);
	PUSH(buttonsCommander, 1)->Setup(32);
	PUSH(buttonsCommander, 2)->Setup(34);
	PUSH(buttonsCommander, 3)->Setup(36);
	PUSH(buttonsCommander, 4)->Setup(38);
	PUSH(buttonsCommander, 5)->Setup(39);
	PUSH(buttonsCommander, 6)->Setup(40);

	// Drivers setups

    // four turnouts are connected to the l293d shield.
	l293d = new DriverL293d();
	l293d->Setup();
	l293d->SetupPortMotor(L293D_PORT_M1, MOTOR12_1KHZ);	//TURNOUT_RIGHT
	l293d->SetupPortMotor(L293D_PORT_M2, MOTOR12_1KHZ);	//TURNOUT_LEFT
	l293d->SetupPortMotor(L293D_PORT_M3, MOTOR34_1KHZ);	//TURNOUT_TJD
	l293d->SetupPortMotor(L293D_PORT_M4, MOTOR34_1KHZ);	//TURNOUT_EPS
	
    // one is on the l298n circuit, with the led.
	l298n = new DriverL298n();
	l298n->Setup();
	l298n->SetupPortMotor(L298N_PORT_OUT1, 50, 52);		//TURNOUT_DC
	l298n->SetupPortMotor(L298N_PORT_OUT2, 46, 48);		//LIGHT1

	// Accessories setups

    // Assign Dcc code for each accessory.
	accessories.Setup(AccessoryNumber);
	accessories.Add(new AccessoryMotorTwoWays(20, 0, 50));	// TURNOUT_LEFT	
	accessories.Add(new AccessoryMotorTwoWays(20, 1, 50));	// TURNOUT_DC
	accessories.Add(new AccessoryMotorTwoWays(21, 0, 300));	// TURNOUT_RIGHT
	accessories.Add(new AccessoryMotorTwoWays(21, 1, 300));	// TURNOUT_EPS
	accessories.Add(new AccessoryMotorTwoWays(22, 0, 300));	// TURNOUT_TJD
	accessories.Add(new AccessoryServo(22, 1, 0));		// SERVO1
	accessories.Add(new AccessoryLight(23, 0, 500));	// LIGHT1

   // Attach each accessory to its driver/port.
	MOTOR2WAYS(accessories, TURNOUT_LEFT)->Setup(l293d, L293D_PORT_M1, 150);
	MOTOR2WAYS(accessories, TURNOUT_RIGHT)->Setup(l293d, L293D_PORT_M2, 150);
	MOTOR2WAYS(accessories, TURNOUT_EPS)->Setup(l293d, L293D_PORT_M3, 150);
	MOTOR2WAYS(accessories, TURNOUT_TJD)->Setup(l293d, L293D_PORT_M4, 150);
	MOTOR2WAYS(accessories, TURNOUT_DC)->Setup(l298n, L298N_PORT_OUT12, 150);
	LIGHT(accessories, LIGHT1)->Setup(l298n, L298N_PORT_OUT34, 150);
	SERVO(accessories, SERVO1)->Setup(l293d, L293D_PORT_SERVO1, 10, 150);

	UAD_EndSetup();
}

void loop()
{
    // Dcc is run first, and if necessary, lets the accessories work.
	if (dccCommander.Loop())
	{
		accessories.Loop();

		dccCommander.Loop();
		buttonsCommander.Loop();
	}
}