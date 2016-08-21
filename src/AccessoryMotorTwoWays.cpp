/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a two ways motorized accessory>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR
#ifndef NO_MOTORTWOWAYS

void AccessoryMotorTwoWays::beginTwoWays(DriverPort *inpPort, unsigned long inIdRight, unsigned long inIdLeft, int speed, unsigned long inDurationMilli)
{
	this->AccessoryMotor::begin(inpPort, inIdRight, speed, inDurationMilli);
	this->AddMovingPosition(inIdLeft, LEFT);
}

#endif
#endif
