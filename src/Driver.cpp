/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Generic driver>
*************************************************************/

#include "Accessories.h"

Driver *Driver::pFirstDriver = 0;
byte Driver::idCounter = 0;

#ifdef DEBUG_MODE
void Driver::CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *inFunc)
{
	return CheckPinNb(GPIO_to_Arduino_pin(inPin), inFunc);
}

void Driver::CheckPinNb(int inPin, const __FlashStringHelper *inFunc)
{
	if (inPin <= 0 || inPin >= NUM_DIGITAL_PINS)
	{
		if (inPin < A0 || inPin > A7)
		{
			Serial.print(F("Pin "));
			Serial.print(inPin);
			Serial.print(F(" error in "));
			Serial.println(inFunc);
		}
	}
}
#endif

Driver::Driver()
{
	AddDriver(this); 
	pNextDriver = 0;
}

void Driver::AddPort(DriverPort *inpPort)
{
	this->Ports.AddItem(*inpPort);
}

DriverPort *Driver::GetPort(PORTTYPE inType, byte inId) const
{
	UADCHAINEDLISTITEM<DriverPort> *pCurr = this->Ports.pFirst;
	while (pCurr != NULL)
	{
		if (pCurr->Obj.GetType() == inType && pCurr->Obj.GetId() == inId)
			return &(pCurr->Obj);
		pCurr = pCurr->pNext;
	}

	return NULL;
}

void Driver::begin()
{
}

void Driver::beginByAccessory(PORTTYPE inType, byte inPort, int inStartingPosition)
{
	this->GetPort(inType, inPort)->beginByAccessory(inStartingPosition);
}

int Driver::SetSpeed(PORTTYPE inType, byte inPort, int inSpeed)
{
	return this->GetPort(inType, inPort)->SetSpeed(inSpeed);
}

void Driver::MoveLeftDir(PORTTYPE inType, byte inPort, unsigned long inDuration, int inSpeed)
{
	DriverPort *port = this->GetPort(inType, inPort);
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = port->SetSpeed(inSpeed);
	port->MoveLeftDir(inDuration);
	if (oldSpeed != -1)
		port->SetSpeed(oldSpeed);
}

void Driver::MoveRightDir(PORTTYPE inType, byte inPort, unsigned long inDuration, int inSpeed)
{
	DriverPort *port = this->GetPort(inType, inPort);
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = port->SetSpeed(inSpeed);
	port->MoveRightDir(inDuration);
	if (oldSpeed != -1)
		port->SetSpeed(oldSpeed);
}

PORT_STATE Driver::MoveToggle(PORTTYPE inType, byte inPort, unsigned long inDuration, int inSpeed)
{
	DriverPort *port = this->GetPort(inType, inPort);
	int oldSpeed = -1;
	if (inSpeed > 0)
		oldSpeed = port->SetSpeed(inSpeed);
	PORT_STATE ret = port->MoveToggle(inDuration);
	if (oldSpeed != -1)
		port->SetSpeed(oldSpeed);
	return ret;
}

void Driver::AddDriver(Driver *inDriver)
{
	if (Driver::pFirstDriver == 0)
	{
		Driver::pFirstDriver = inDriver;
		return;
	}

	Driver *pCurr = Driver::pFirstDriver;

	while (pCurr->pNextDriver != 0)
		pCurr = pCurr->pNextDriver;

	pCurr->pNextDriver = inDriver;
}

void Driver::loops()
{
	Driver *pCurr = Driver::pFirstDriver;

	while (pCurr != 0)
	{
		//pCurr->loop();
		pCurr = pCurr->pNextDriver;
	}
}


