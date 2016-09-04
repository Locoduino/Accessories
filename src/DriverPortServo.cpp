/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a servo>
*************************************************************/

#include "Accessories.h"

#ifndef NO_SERVO

DriverPortServo::DriverPortServo(uint8_t inId) : DriverPort(SERVO, inId)
{
}

void DriverPortServo::begin()
{
}

void DriverPortServo::beginByAccessory(int inStartingPosition)
{
}

void DriverPortServo::MovePosition(unsigned long inDuration, int inEndPosition)
{
}

int DriverPortServo::GetPosition()
{
	return 0;
}
#endif