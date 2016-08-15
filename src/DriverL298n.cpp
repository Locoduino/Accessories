/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a L298n driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_L298N

DriverL298n::DriverL298n()
{
#ifndef NO_MOTOR_LIGHT
	this->AddPort(new DriverPortL298n(L298N_PORT_OUT12));
	this->AddPort(new DriverPortL298n(L298N_PORT_OUT34));
#endif
}

DriverPortL298n *DriverL298n::beginPortMotor(byte inPort, int inPinA, int inPinB)
{
#ifndef NO_MOTOR_LIGHT
	DriverPortL298n *pPort = (DriverPortL298n *) this->GetPort(MOTOR_LIGHT, inPort);
	pPort->begin(inPinA, inPinB);
	return pPort;
#endif
}

/*DriverPortStepper *DriverL298n::beginPortStepper(byte inPort, int inPinA, int inPinB)
{
#ifndef NO_STEPPER
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, inPort);
	pPort->begin(inPinA, inPinB);
	return pPort;
#endif
}
*/

#endif
