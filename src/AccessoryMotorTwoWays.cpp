/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a two ways motorized accessory>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR
#ifndef NO_MOTORTWOWAYS

AccessoryMotorTwoWays::AccessoryMotorTwoWays(unsigned long inId, unsigned long inDurationMilli)// : AccessoryMotor(inId, inDurationMilli)
{
}

AccessoryMotorTwoWays::AccessoryMotorTwoWays(unsigned long inIdRight, unsigned long inIdLeft, unsigned long inDurationMilli)// : AccessoryMotor(inIdRight, inDurationMilli)
{
	this->AddMovingPosition(inIdLeft, LEFT);
}

#endif
#endif
