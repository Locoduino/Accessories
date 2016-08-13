/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a relay driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_RELAY
#ifndef NO_MOTOR_LIGHT

// Only height for maxi size, but it can be increased if you have memory space...

DriverRelay::DriverRelay(unsigned char inPortsNb)
{
	for(unsigned char i = 0; i < inPortsNb; i++)
		this->AddPort(new DriverPortRelay(i));
}

void DriverRelay::beginPort(unsigned char inPort, int inPin)
{
	((DriverPortRelay *)(this->GetPort(MOTOR_LIGHT, inPort)))->begin(inPin);
}

#endif
#endif