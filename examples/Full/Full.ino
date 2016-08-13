/*************************************************************
project: <DCC Accessory Decoder>
author: <Thierry PARIS>
description: <2 servos and 3 leds driven by dcc arduino>
*************************************************************/

#include "UniversalAccessoryDecoder.h"

/* kDCC_INTERRUPT values :
Board         int.0   int.1   int.2   int.3   int.4   int.5
Uno, Ethernet   2      3
Mega2560        2      3      21      20      19      18
Leonardo        3      2      0       1       7
*/
#define kDCC_INTERRUPT            3

///////////////////////////////////////////////////////////////
// The target is to move 5 servos, by DCC adress

#define AccessoryServoNumber      2

#define SERVO1			0
#define SERVO2			1

#define SERVO1_PORT		0
#define SERVO2_PORT		1

#define AccessoryMotorLightNumber      3

#define LIGHT1			2
#define LIGHT2			3
#define LIGHT3			4

#define LIGHT1_PORT		0
#define LIGHT2_PORT		1
#define LIGHT3_PORT		2

// Accessories

Accessories accessories;
DccCommander dccCommander;
ButtonsCommander buttonsCommander;

AccessoryGroup groupServos;
AccessoryGroup groupLights;

// Drivers

DriverArduino *arduino;

//////////////////////////////////
//
// Setup
//
void setup()
{
	UAD_StartSetup();

    // Setup of Dcc commander
	dccCommander.Setup(0x00, 0x00, kDCC_INTERRUPT);
	dccCommander.SetStatusLedPin(13);

    // Three buttons in this commander
	buttonsCommander.Setup(3,
		new ButtonsCommanderPush(2),
		new ButtonsCommanderSwitch(2),
		new ButtonsCommanderPotentiometer(321, 0, 20, 145)	// Link it to SERVO1, from 20 to 145 degrees
		);
    // The push button send two Dcc codes in turn, and is connected to pin 26.
	PUSH(buttonsCommander, 0)->AddDccId(320, 1);
	PUSH(buttonsCommander, 0)->AddDccId(320, 0);
	PUSH(buttonsCommander, 0)->Setup(26);

    // The switch have two positions, each one controls one Dcc code. Each one connected to its pin.
	SWITCH(buttonsCommander, 1)->AddDccId(319, 0, 24);
	SWITCH(buttonsCommander, 1)->AddDccId(319, 1, 25);
	SWITCH(buttonsCommander, 1)->Setup();

    // The potentiometer is on analog pin 8.
	POTENTIOMETER(buttonsCommander, 2)->Setup(8);

	// Drivers setups

    // List of the ports on the Arduino. Pors 9,10 and 11 are handled in analog mode for fading.
	arduino = new DriverArduino(AccessoryMotorLightNumber, AccessoryServoNumber);
	arduino->Setup();
	arduino->SetupPortServo(SERVO1_PORT, 2);
	arduino->SetupPortServo(SERVO2_PORT, 3);
	arduino->SetupPortMotor(LIGHT1_PORT, 9, ANALOG);
	arduino->SetupPortMotor(LIGHT2_PORT, 10, ANALOG);
	arduino->SetupPortMotor(LIGHT3_PORT, 11, ANALOG);

	// Accessories setups

    // Declare accessories with Dcc codes.
	accessories.Setup(
		AccessoryMotorLightNumber + AccessoryServoNumber,
            // Servo1 will be activated with only one Dcc code 316/0 used as a toggle.
            // 20ms is the duration of one degree movement.
		new AccessoryServo(316, 0, 20),
            // Servo2 use two Dcc codes: 314/0 for minimum position and 314/1 for maximum.
		new AccessoryServo(314, 0, 314, 1, 20),
		new AccessoryLight(1, 0),
		new AccessoryLight(1, 1),
		new AccessoryLight(2, 0)
		);
    
    // Attach the servos to their driver/ports.
    // Servo1 can move from 20 to 145 degrees.
	SERVO(accessories, SERVO1)->Setup(arduino, SERVO1_PORT, 20, 145);

    // Servo2 can move from 10 to 150 degrees. But there is also two more positions at 45 and 135 degrees,
    // commanded by Dcc codes 315/0 and 315/1.
	SERVO(accessories, SERVO2)->Setup(arduino, SERVO2_PORT, 10, 150, 4);
	SERVO(accessories, SERVO2)->AddDccPosition(315, 0, 45);
	SERVO(accessories, SERVO2)->AddDccPosition(315, 1, 135);

    // Servo1 has a pin 49 to control a relay giving power to the servo.
	SERVO(accessories, SERVO1)->SetPowerCommand(49);

    // Declare light fading/dimming.
	LIGHT(accessories, LIGHT1)->SetFading(20, 10);
	LIGHT(accessories, LIGHT2)->SetFading(20, 10);
	//LIGHT(accessories, LIGHT3)->SetFading(20, 10);

    // Attach the lights to their driver/ports.
	LIGHT(accessories, LIGHT1)->Setup(arduino, LIGHT1_PORT);
	LIGHT(accessories, LIGHT2)->Setup(arduino, LIGHT2_PORT);
	LIGHT(accessories, LIGHT3)->Setup(arduino, LIGHT3_PORT);

    // One group state with servos at minimum is created, activated by 320/0 Dcc code.
	GroupState *pServo1 = new GroupState(320, 0, false);
	pServo1->Setup(2,
		new GroupStateItem(accessories[SERVO1], MINIMUM, 500),
		new GroupStateItem(accessories[SERVO2], MINIMUM, 500));

    // Another group state with servos at maximum is created, activated by 320/1 Dcc code.
	GroupState *pServo2 = new GroupState(320, 1, false);
	pServo2->Setup(2,
		new GroupStateItem(accessories[SERVO1], MAXIMUM, 500),
		new GroupStateItem(accessories[SERVO2], MAXIMUM, 500));

    // The two states are gourped in a ... group !
	groupServos.Setup(2, pServo1, pServo2);

    // One group state with lights on/off/on, for Dcc code 319/0.
	GroupState *pLight1 = new GroupState(319, 0, true);
	pLight1->Setup(3,
		new GroupStateItem(accessories[LIGHT1], LIGHTON),
		new GroupStateItem(accessories[LIGHT2], LIGHTOFF),
		new GroupStateItem(accessories[LIGHT3], LIGHTON));

    // The other group state with lights off/of/off, for Dcc code 319/1.
	GroupState *pLight2 = new GroupState(319, 1, true);
	pLight2->Setup(3,
		new GroupStateItem(accessories[LIGHT1], LIGHTOFF),
		new GroupStateItem(accessories[LIGHT2], LIGHTON),
		new GroupStateItem(accessories[LIGHT3], LIGHTOFF));

    // One group state with lights on/off/on, for Dcc code 319/0.
	groupLights.Setup(2, pLight1, pLight2);

	UAD_EndSetup();
}

void loop()
{
    // Run the Loop on all items, if Dcc Loop agrees !
	if (dccCommander.Loop())
	{
		accessories.Loop();
		groupServos.Loop();
		groupLights.Loop();

		buttonsCommander.Loop();
	}
}