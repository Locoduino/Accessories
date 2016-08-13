/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port>
*************************************************************/

#include "Accessories.h"

DriverPort::DriverPort(PORTTYPE inType, byte inPort)
{
	this->type = inType;
	this->id = inPort;
	this->state = PORT_STOP;
	this->speed = DEFAULTSPEED;
}

int DriverPort::SetSpeed(int inSpeed)
{
	uint8_t oldSpeed = this->speed;
	this->speed = inSpeed;
	return oldSpeed;
}

PORT_STATE DriverPort::MoveToggle(unsigned long inDuration)
{
	if (this->IsLeftDir())
	{
		MoveRightDir(inDuration);
	}
	else
	{
		MoveLeftDir(inDuration);
	}
		
	return this->GetState();
}

void DriverPort::MoveLeftDir(unsigned long inDuration, int inSpeed)
{
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = this->SetSpeed(inSpeed);
	this->MoveLeftDir(inDuration);
	if (oldSpeed != -1)
		this->SetSpeed(oldSpeed);
}

void DriverPort::MoveRightDir(unsigned long inDuration, int inSpeed)
{
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = this->SetSpeed(inSpeed);
	this->MoveRightDir(inDuration);
	if (oldSpeed != -1)
		this->SetSpeed(oldSpeed);
}

