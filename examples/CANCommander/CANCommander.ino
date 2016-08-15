/*************************************************************
project: <Carte 8 Servos Locoduino by UAD>
author: <Thierry PARIS>
*************************************************************/

#include <Commanders.h>
#include <Accessories.h>

#define kDCC_INTERRUPT      0

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
	Commanders::SetEventHandler(ReceiveEvent);
	Commanders::SetStatusLedPin(LED_BUILTIN);

	DccCommander.begin(0x00, 0x00, kDCC_INTERRUPT);

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

	servo0.begin(pPort0, speed, 0, 180, 2);
	servo1.begin(pPort1, speed, 0, 180, 2);
	servo2.begin(pPort2, speed, 0, 180, 2);
	servo3.begin(pPort3, speed, 0, 180, 2);
	servo4.begin(pPort4, speed, 0, 180, 2);
	servo5.begin(pPort5, speed, 0, 180, 2);
	servo6.begin(pPort6, speed, 0, 180, 2);
	servo7.begin(pPort7, speed, 0, 180, 2);

	servo0.AddMinMaxMovingPositions(DCCINT(30, 0), DCCINT(30, 1));
	servo1.AddMinMaxMovingPositions(DCCINT(31, 0), DCCINT(31, 1));
	servo2.AddMinMaxMovingPositions(DCCINT(32, 0), DCCINT(32, 1));
	servo3.AddMinMaxMovingPositions(DCCINT(33, 0), DCCINT(33, 1));
	servo4.AddMinMaxMovingPositions(DCCINT(34, 0), DCCINT(34, 1));
	servo5.AddMinMaxMovingPositions(DCCINT(35, 0), DCCINT(35, 1));
	servo6.AddMinMaxMovingPositions(DCCINT(36, 0), DCCINT(36, 1));
	servo7.AddMinMaxMovingPositions(DCCINT(37, 0), DCCINT(37, 1));
}

void loop()
{
	Commanders::loop();
	Accessories::loop();
}
