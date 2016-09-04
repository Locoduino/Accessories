/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a LMD18200 driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_LMD18200

DriverLMD18200::DriverLMD18200()
{
#ifndef NO_MOTOR_LIGHT
	this->AddPort(new DriverPortLMD18200(0));
#endif
}

DriverPortLMD18200 *DriverLMD18200::beginPortMotor(int inPinPWM, int inPinDir, int inPinBrake)
{
#ifndef NO_MOTOR_LIGHT
	DriverPortLMD18200 *pPort = (DriverPortLMD18200 *) this->GetPort(MOTOR_LIGHT, 0);
	pPort->begin(inPinPWM, inPinDir, inPinBrake);
	return pPort;
#endif
}

#endif
