/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Demo for servo>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            5

///////////////////////////////////////////////////////////////
// The target is to move a servo, allowing four buttons to change
// driven mode (fast, slow exclusive, memo and very slow).
// And four other buttons to reach four different positions.
// A classic button also toggle between min and max positions.

// total number of accessories.
#define AccessoryNumber		1

#define SERVO1				0

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

// Drivers

// Only one servo dirver here, a l293d shield,
// but we could use the arduino itself...	
DriverL293d *arduino;

// current user define speed.
int speed;

// servo accessory.
AccessoryServo *servo;

//////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dccCommander.SetStatusLedPin(13);

	buttonsCommander.Setup(9,
		new ButtonsCommanderPush(1),	// Classic action button
		new ButtonsCommanderPush(1),	// Change position #1
		new ButtonsCommanderPush(1),	// Change position #2
		new ButtonsCommanderPush(1),	// Change position #3
		new ButtonsCommanderPush(1),	// Change position #4
		new ButtonsCommanderPush(1),	// Change mode #1
		new ButtonsCommanderPush(1),	// Change mode #2
		new ButtonsCommanderPush(1),	// Change mode #3
		new ButtonsCommanderPush(1)		// Change mode #4
		);
	PUSH(buttonsCommander, 0)->AddDccId(22, 0);	// Classic action button
	PUSH(buttonsCommander, 0)->AddDccId(30, 0);	// Change position #1
	PUSH(buttonsCommander, 0)->AddDccId(30, 1);	// Change position #2
	PUSH(buttonsCommander, 0)->AddDccId(31, 0);	// Change position #3
	PUSH(buttonsCommander, 0)->AddDccId(31, 1);	// Change position #4
	PUSH(buttonsCommander, 0)->AddDccId(101, 0);// Change mode #1
	PUSH(buttonsCommander, 0)->AddDccId(102, 0);// Change mode #2
	PUSH(buttonsCommander, 0)->AddDccId(103, 0);// Change mode #3
	PUSH(buttonsCommander, 0)->AddDccId(104, 0);// Change mode #4

	PUSH(buttonsCommander, 0)->Setup(40);	// Classic action button
	PUSH(buttonsCommander, 0)->Setup(30);	// Change position #1
	PUSH(buttonsCommander, 0)->Setup(31);	// Change position #2
	PUSH(buttonsCommander, 0)->Setup(32);	// Change position #3
	PUSH(buttonsCommander, 0)->Setup(33);	// Change position #4
	PUSH(buttonsCommander, 0)->Setup(20);	// Change mode #1
	PUSH(buttonsCommander, 0)->Setup(21);	// Change mode #2
	PUSH(buttonsCommander, 0)->Setup(22);	// Change mode #3
	PUSH(buttonsCommander, 0)->Setup(23);	// Change mode #4

	// Drivers setups

	arduino = new DriverL293d();
	arduino->Setup();

	// Buttons setups

	speed = 19;	// starting speed state : fast.

	// Accessories setups

	accessories.Setup(AccessoryNumber, 
		new AccessoryServo(30, 0, 30, 1, speed));	// Servo with DCC id for minimum AND maximum

	SERVO(accessories, SERVO1)->AddDccPosition(31, 0, 30);	// One more Dcc id for angle 30
	SERVO(accessories, SERVO1)->AddDccPosition(31, 1, 90);	// One more Dcc id for angle 90

	SERVO(accessories, SERVO1)->Setup(arduino, L293D_PORT_SERVO1, 10, 160);
	
	UAD_EndSetup();
}

void loop()
{
	if (dccCommander.Loop())
	{
		accessories.Loop();
		if (!buttonsCommander.Loop())
		{
			// If ButtonsCommander have found no correspondance between an accessory or a group state
			// handle it manually.
#ifdef DEBUG_MODE
			Serial.print(F("Mode button :"));
			Serial.println(buttonsCommander.GetLastSelectedButton()->GetDccIdDecoder());
#endif
			switch (buttonsCommander.GetLastSelectedButton()->GetDccIdDecoder())
			{
			case 101:	// Fast
				servo->SetDuration(0);
				break;
			case 102:	// Slow exclusive
				servo->SetDuration(5);
				break;
			case 103: // Slow with event memo
				servo->SetDuration(10);
				break;
			case 104: // Classic Slow. 
				servo->SetDuration(30);
				break;
			}
		}
	}
}
