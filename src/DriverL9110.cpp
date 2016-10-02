/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a L9110 driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_L9110

DriverL9110::DriverL9110()
{
#if !defined(NO_MOTOR_LIGHT)
	this->AddPort(new DriverPort2Pins(0));
#endif

#if !defined(NO_SERVO)
	this->AddPort(new DriverPortServoBase(0));
#endif
}

void DriverL9110::begin()
{
	Driver::begin();
}

DriverPort2Pins *DriverL9110::beginPortMotor(uint8_t inPinA, int inPinB)
{
#if !defined(NO_MOTOR_LIGHT)
	DriverPort2Pins *pPort = (DriverPort2Pins *) this->GetPort(MOTOR_LIGHT, 0);
	pPort->begin(inPinA, inPinB);
	return pPort;
#endif
}

DriverPortServoBase * DriverL9110::beginPortServo(uint8_t inPinA)
{
#if !defined(NO_SERVO)
	DriverPortServoBase *pPort = (DriverPortServoBase *) this->GetPort(SERVO, 0);
	pPort->begin(inPinA, PORT_TYPE::ANALOG);
	return pPort;
#endif
}
/*
DriverPortStepper * DriverL9110::beginPortStepper(uint8_t inPort)
{
#if !defined(NO_STEPPER)
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, inPort);
	pPort->begin(0);
	return pPort;
#endif
}
*/

#endif
