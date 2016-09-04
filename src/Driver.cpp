/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Generic driver>
*************************************************************/

#include "Accessories.h"

Driver *Driver::pFirstDriver = 0;
uint8_t Driver::idCounter = 0;

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
void Driver::CheckPinNb2(GPIO_pin_t inPin, const char *inFunc)
{
	return CheckPinNb(GPIO_to_Arduino_pin(inPin), inFunc);
}

void Driver::CheckPinNb(int inPin, const char *inFunc)
{
	if (inPin <= 0 || inPin >= (int)NUM_DIGITAL_PINS)
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
#else
void Driver::CheckPinNb2(GPIO_pin_t inPin, const __FlashStringHelper *inFunc)
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
#endif

Driver::Driver()
{
	AddDriver(this); 
	pNextDriver = 0;
}

void Driver::AddPort(DriverPort *inpPort)
{
	this->Ports.AddItem(inpPort);
}

DriverPort *Driver::GetPort(PORTTYPE inType, uint8_t inId) const
{
	ACCSCHAINEDLISTITEM<DriverPort> *pCurr = this->Ports.pFirst;
	while (pCurr != NULL)
	{
		if (pCurr->Obj->GetType() == inType && pCurr->Obj->GetId() == inId)
			return pCurr->Obj;
		pCurr = pCurr->pNext;
	}

	return NULL;
}

void Driver::begin()
{
}

void Driver::beginByAccessory(PORTTYPE inType, uint8_t inPort, int inStartingPosition)
{
	this->GetPort(inType, inPort)->beginByAccessory(inStartingPosition);
}

int Driver::SetSpeed(PORTTYPE inType, uint8_t inPort, int inSpeed)
{
	return this->GetPort(inType, inPort)->SetSpeed(inSpeed);
}

void Driver::MoveLeftDir(PORTTYPE inType, uint8_t inPort, unsigned long inDuration, int inSpeed)
{
	DriverPort *port = this->GetPort(inType, inPort);
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = port->SetSpeed(inSpeed);
	port->MoveLeftDir(inDuration);
	if (oldSpeed != -1)
		port->SetSpeed(oldSpeed);
}

void Driver::MoveRightDir(PORTTYPE inType, uint8_t inPort, unsigned long inDuration, int inSpeed)
{
	DriverPort *port = this->GetPort(inType, inPort);
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = port->SetSpeed(inSpeed);
	port->MoveRightDir(inDuration);
	if (oldSpeed != -1)
		port->SetSpeed(oldSpeed);
}

PORT_STATE Driver::MoveToggle(PORTTYPE inType, uint8_t inPort, unsigned long inDuration, int inSpeed)
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
	if (Driver::pFirstDriver == NULL)
	{
		Driver::pFirstDriver = inDriver;
		return;
	}

	Driver *pCurr = Driver::pFirstDriver;

	while (pCurr->pNextDriver != NULL)
		pCurr = pCurr->pNextDriver;

	pCurr->pNextDriver = inDriver;
}



