/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
*************************************************************/

//#include "UniversalAccessoryDecoderV5.h"
#ifdef VISUALSTUDIO
#include "../../Commanders/src/Commanders.h"
#else
#include <Commanders.h>
#endif

#define kDCC_INTERRUPT      0

#define AccessoryNumber		8

#define SERVO0				0
#define SERVO1				1
#define SERVO2				2
#define SERVO3				3
#define SERVO4				4
#define SERVO5				5
#define SERVO6				6
#define SERVO7				7

DccCommander dccCommander;
CANCommander canCommander;
I2CCommander i2cCommander;

// Drivers

//DriverArduino *arduino;

// current user define speed.
int speed;

// servo accessory.
//AccessoryServo *servo;

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	//UAD_ReceiveEvent(inId, (byte) inEventType, inEventData);
	
    Commanders.printEvent(inId, inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Commanders::StartSetup(ReceiveEvent, 13);
//	UAD_StartSetup();

	dccCommander.begin(0x00, 0x00, kDCC_INTERRUPT);

	canCommander.begin(10, CAN_125KBPS, digitalPinToInterrupt(3));

/*	// Drivers setups

	arduino = new DriverArduino(0, 8);
	arduino->begin();

	arduino->beginPortServo(SERVO0, 6);
	arduino->beginPortServo(SERVO1, 7);
	arduino->beginPortServo(SERVO2, 8);
	arduino->beginPortServo(SERVO3, 9);
	arduino->beginPortServo(SERVO4, A0);
	arduino->beginPortServo(SERVO5, A1);
	arduino->beginPortServo(SERVO6, A2);
	arduino->beginPortServo(SERVO7, A3);

	speed = 19;	// starting speed state : fast.

				// Accessories setups

	Accessories.begin();
	AccessoryServo *pServo0 = new AccessoryServo(DCCINT(30, 0), DCCINT(30, 1), speed);	// Servo with DCC id for minimum AND maximum
	AccessoryServo *pServo1 = new AccessoryServo(DCCINT(31, 0), DCCINT(31, 1), speed);
	AccessoryServo *pServo2 = new AccessoryServo(DCCINT(32, 0), DCCINT(32, 1), speed);
	AccessoryServo *pServo3 = new AccessoryServo(DCCINT(33, 0), DCCINT(33, 1), speed);
	AccessoryServo *pServo4 = new AccessoryServo(DCCINT(34, 0), DCCINT(34, 1), speed);
	AccessoryServo *pServo5 = new AccessoryServo(DCCINT(35, 0), DCCINT(35, 1), speed);
	AccessoryServo *pServo6 = new AccessoryServo(DCCINT(36, 0), DCCINT(36, 1), speed);
	AccessoryServo *pServo7 = new AccessoryServo(DCCINT(37, 0), DCCINT(37, 1), speed);

	pServo0->begin(arduino, 0);
	pServo1->begin(arduino, 1);
	pServo2->begin(arduino, 2);
	pServo3->begin(arduino, 3);
	pServo4->begin(arduino, 4);
	pServo5->begin(arduino, 5);
	pServo6->begin(arduino, 6);
	pServo7->begin(arduino, 7);

	UAD_EndSetup();
	*/
	Commanders::EndSetup();
}

void loop()
{
	Commanders::loop();
//	Accessories.loops();
}
