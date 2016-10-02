/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a L293d driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_L293D

DriverL293d::DriverL293d()
{
#ifndef NO_MOTOR_LIGHT
	this->AddPort(new DriverPort2PinsEnable(L293D_PORT_OUT12));
	this->AddPort(new DriverPort2PinsEnable(L293D_PORT_OUT34));
#endif
#ifndef NO_STEPPER
	this->AddPort(new DriverPortStepper(0));
#endif
}

DriverPort2PinsEnable *DriverL293d::beginPortMotor(uint8_t inPort, int inPinA, int inPinB, int inPinEnable)
{
#ifndef NO_MOTOR_LIGHT
	DriverPort2PinsEnable *pPort = (DriverPort2PinsEnable *) this->GetPort(MOTOR_LIGHT, inPort);
	pPort->begin(inPinA, inPinB, inPinEnable);
	return pPort;
#endif
}

DriverPortStepper *DriverL293d::beginPortStepper(int inPinA, int inPinB, int inPinC, int inPinD, uint8_t *inpSteps)
{
#ifndef NO_STEPPER
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, 0);
	pPort->begin(inPinA, inPinB, inPinC, inPinD, inpSteps);
	return pPort;
#endif
}


#endif
