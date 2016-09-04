/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a L298n driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_ULN2003

DriverULN2003::DriverULN2003()
{
#if !defined(NO_MOTOR_LIGHT)
	// Eight motor/led ports available on this driver
	for (int i = 1; i < 8; i++)
		this->AddPort(new DriverPortMotor(i));
#endif

#if !defined(NO_SERVO)
	// Eight servo ports available on this driver (the same as motor/led !)
	for (int i = 1; i < 8; i++)
		this->AddPort(new DriverPortServoBase(i));
#endif

#if !defined(NO_STEPPER)
	// Up to three steppers can be connected by two wires, but only one if connected by four or five wires !
	this->AddPort(new DriverPortStepper(1));
	this->AddPort(new DriverPortStepper(2));
	this->AddPort(new DriverPortStepper(3));
#endif
}

#ifndef NO_MOTOR_LIGHT
DriverPortMotor *DriverULN2003::beginPortMotor(uint8_t inPort, int inPin)
{
	DriverPortMotor *pPort = (DriverPortMotor *) this->GetPort(MOTOR_LIGHT, inPort);
	pPort->begin(inPin);
	return pPort;
}
#endif

#ifndef NO_SERVO
DriverPortServo *DriverULN2003::beginPortServo(uint8_t inPort, int inPin)
{
	DriverPortServoBase *pPort = (DriverPortServoBase *) this->GetPort(SERVO, inPort);
	pPort->begin(inPin, ANALOG);
	return pPort;
}
#endif

#ifndef NO_STEPPER
DriverPortStepper *DriverULN2003::beginPortStepper(uint8_t inPort, int inPin1, int inPin2, uint8_t *inpSteps)
{
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, inPort);
	pPort->begin(inPin1, inPin2, inpSteps);
	return pPort;
}

DriverPortStepper *DriverULN2003::beginPortStepper(uint8_t inPort, int inPin1, int inPin2, int inPin3, int inPin4, uint8_t *inpSteps)
{
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, inPort);
	pPort->begin(inPin1, inPin2, inPin3, inPin4, inpSteps);
	return pPort;
}
#endif

#endif
