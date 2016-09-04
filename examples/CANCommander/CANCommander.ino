/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
*************************************************************/

#include <Commanders.h>
#include <Accessories.h>

#define SERVO0_MIN	DCCINT(20, 0)
#define SERVO0_MAX	DCCINT(20, 1)
#define SERVO1_MIN	DCCINT(21, 0)
#define SERVO1_MAX	DCCINT(21, 1)
#define SERVO2_MIN	DCCINT(22, 0)
#define SERVO2_MAX	DCCINT(22, 1)
#define SERVO3_MIN	DCCINT(23, 0)
#define SERVO3_MAX	DCCINT(23, 1)
#define SERVO4_MIN	DCCINT(24, 0)
#define SERVO4_MAX	DCCINT(24, 1)
#define SERVO5_MIN	DCCINT(25, 0)
#define SERVO5_MAX	DCCINT(25, 1)
#define SERVO6_MIN	DCCINT(26, 0)
#define SERVO6_MAX	DCCINT(26, 1)
#define SERVO7_MIN	DCCINT(27, 0)
#define SERVO7_MAX	DCCINT(27, 1)

// Commanders

SERIAL_COMMANDER(Serial);

// Drivers

DriverArduino arduino;

// current user define speed.
int speed;

// servo accessory.
AccessoryServo servo0;
AccessoryServo servo1;
AccessoryServo servo2;
AccessoryServo servo3;
AccessoryServo servo4;
AccessoryServo servo5;
AccessoryServo servo6;
AccessoryServo servo7;

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Accessories::ReceiveEvent(inId, (ACCESSORIES_EVENT_TYPE) inEventType, inEventData);
	
    Commanders::printEvent(inId, inEventType, inEventData);
}

//////////////////////////////////
//
// Setup
//
void setup()
{
	Serial.begin(115200);
	while (!Serial);		// For Leonardo only. No effect on other Arduino.

	Commanders::begin(ReceiveEvent, LED_BUILTIN);
	Accessories::begin();

	// Commanders setup

	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(2));
	SerialCommander.begin();

	CANCommander.begin(10, CAN_125KBPS, digitalPinToInterrupt(3), 100);

	// Drivers setups

	arduino.begin();

	DriverPortServo *pPort0 = arduino.AddPortServo(6);
	DriverPortServo *pPort1 = arduino.AddPortServo(7);
	DriverPortServo *pPort2 = arduino.AddPortServo(8);
	DriverPortServo *pPort3 = arduino.AddPortServo(9);
	DriverPortServo *pPort4 = arduino.AddPortServo(A0);
	DriverPortServo *pPort5 = arduino.AddPortServo(A1);
	DriverPortServo *pPort6 = arduino.AddPortServo(A2);
	DriverPortServo *pPort7 = arduino.AddPortServo(A3);

	// Accessories setups

	speed = 19;	// starting speed state : fast.

	servo0.begin(pPort0, speed, 0, 10, 2);
	servo1.begin(pPort1, speed, 0, 10, 2);
	servo2.begin(pPort2, speed, 0, 10, 2);
	servo3.begin(pPort3, speed, 0, 10, 2);
	servo4.begin(pPort4, speed, 0, 10, 2);
	servo5.begin(pPort5, speed, 0, 10, 2);
	servo6.begin(pPort6, speed, 0, 10, 2);
	servo7.begin(pPort7, speed, 0, 10, 2);

	servo0.AddMinMaxMovingPositions(SERVO0_MIN, SERVO0_MAX);
	servo1.AddMinMaxMovingPositions(SERVO1_MIN, SERVO1_MAX);
	servo2.AddMinMaxMovingPositions(SERVO2_MIN, SERVO2_MAX);
	servo3.AddMinMaxMovingPositions(SERVO3_MIN, SERVO3_MAX);
	servo4.AddMinMaxMovingPositions(SERVO4_MIN, SERVO4_MAX);
	servo5.AddMinMaxMovingPositions(SERVO5_MIN, SERVO5_MAX);
	servo6.AddMinMaxMovingPositions(SERVO6_MIN, SERVO6_MAX);
	servo7.AddMinMaxMovingPositions(SERVO7_MIN, SERVO7_MAX);
}

void loop()
{
	Commanders::loop();
	Accessories::loop();
}
