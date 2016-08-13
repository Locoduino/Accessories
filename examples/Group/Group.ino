/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <Demo for groups>
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
// The target is to simulate a traffic light with four leds.
// A group defines the traffic light, which link four light accessories.
// Four different states defines which light must be on, which must be off.
// Only one button is used to move from one state to the following,
// returning to the first one when finish.

// total number of accessories.
#define AccessoryNumber		4

#define RED			0
#define GREEN			1
#define ORANGE			2
#define WHITE			3

// DCC codes
#define ALLON			15,0
#define ALLOFF			15,1
#define ALLONFIRSTBLINK	16,0
#define ALLOFFLASTBLINK	16,1

// Accessories

Accessories accessories;
AccessoryGroup mainGroup;
ButtonsCommander buttonsCommander;

/////////////////////////////////////////////////////////////
// Defines here the four states of this group.
// Each state can be affected to a Dcc Id (first argument, here 0).
// 'true' as second argumentif all changes due to rich this state
// can be done simultaneously. If false, if power is not enough 
// to do all at the same time for instance, all changes will be done
// sequentially.

// Drivers
	
DriverArduino *arduino;

//////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

	buttonsCommander.Setup(1,
		new ButtonsCommanderPush(4)
		);
	PUSH(buttonsCommander, 0)->AddDccId(ALLON);
	PUSH(buttonsCommander, 0)->AddDccId(ALLOFF);
	PUSH(buttonsCommander, 0)->AddDccId(ALLONFIRSTBLINK);
	PUSH(buttonsCommander, 0)->AddDccId(ALLOFFLASTBLINK);

	PUSH(buttonsCommander, 0)->Setup(17);

	// Drivers setups

	// Wire leds to pins 8, 9, 10, 11

	arduino = new DriverArduino(4, 0);
	arduino->Setup();
	arduino->SetupPortMotor(RED, 8);
	arduino->SetupPortMotor(GREEN, 9);
	arduino->SetupPortMotor(ORANGE, 10);
	arduino->SetupPortMotor(WHITE, 11);
	
	// Buttons setups

	// Accessories setups

	// Build the list of accessories : four leds
	// The first argument 0 is the Dcc id, 0 here to be sure no Dcc paquet
	// will affect directly the traffic light via the accessories.
	// 1000 means that when blinking, they will do with 1 second frequency !
	accessories.Setup(AccessoryNumber,
		new AccessoryLight(20, 0, 500),		//RED
		new AccessoryLight(20, 1),		//GREEN
		new AccessoryLight(21, 0),		//ORANGE
		new AccessoryLight(21, 1, 300));	//WHITE

	// Declare the link between each accessory and its driver / port
	// Each led index is also the port number : led 0 or port 0, led 1 on port 1, etc..
	LIGHT(accessories, RED)->Setup(arduino, RED);
	LIGHT(accessories, GREEN)->Setup(arduino, GREEN);
	LIGHT(accessories, ORANGE)->Setup(arduino, ORANGE);
	LIGHT(accessories, WHITE)->Setup(arduino, WHITE);

	// Describe the differents states
	// All with all lights on
	GroupState *pAll = new GroupState(ALLON, false);
	pAll->Setup(4,
		new GroupStateItem(accessories[RED], LIGHTON),
		new GroupStateItem(accessories[GREEN], LIGHTON),
		new GroupStateItem(accessories[ORANGE], LIGHTON),
		new GroupStateItem(accessories[WHITE], LIGHTON));
	
	// None with all ligths off
	GroupState *pNone = new GroupState(ALLOFF, false);
	pNone->Setup(4,
		new GroupStateItem(accessories[RED], LIGHTOFF),
		new GroupStateItem(accessories[GREEN], LIGHTOFF),
		new GroupStateItem(accessories[ORANGE], LIGHTOFF),
		new GroupStateItem(accessories[WHITE], LIGHTOFF));
	
	// All lights on with red blinking
	GroupState *pAllAndRedFlashing = new GroupState(ALLONFIRSTBLINK, false);
	pAllAndRedFlashing->Setup(4,
		new GroupStateItem(accessories[RED], LIGHTBLINK),
		new GroupStateItem(accessories[GREEN], LIGHTON),
		new GroupStateItem(accessories[ORANGE], LIGHTON),
		new GroupStateItem(accessories[WHITE], LIGHTON));
	
	// All lights off with white blinking
	GroupState *pNoneAndWhiteFlashing = new GroupState(ALLOFFLASTBLINK, false);
	pNoneAndWhiteFlashing->Setup(4,
		new GroupStateItem(accessories[RED], LIGHTOFF),
		new GroupStateItem(accessories[GREEN], LIGHTOFF),
		new GroupStateItem(accessories[ORANGE], LIGHTOFF),
		new GroupStateItem(accessories[WHITE], LIGHTBLINK));
	
	// Gives to the group the list and the order of states.
	mainGroup.Setup(4, pNone, pAllAndRedFlashing, pNoneAndWhiteFlashing, pAll);

	UAD_EndSetup();

	mainGroup.DCCToggle(ALLOFF);
}

void loop()
{
	accessories.Loop();
	mainGroup.Loop();
	buttonsCommander.Loop();
}
