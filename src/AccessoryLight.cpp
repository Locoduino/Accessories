/**********************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a light, flashing or not, with optional fading>
***********************************************************************/

#include "Accessories.h"
#include "AccessoryLight.hpp"

#ifndef NO_LIGHT

AccessoryLight::AccessoryLight()
{
}

void AccessoryLight::begin(Port *inpPort, unsigned long inId, unsigned long inBlinkDuration, int inIntensity)
{ 
	this->pPort = inpPort;
	this->pLight = new AccessoryBaseLight(this);

	this->pLight->SetBlinking(inBlinkDuration);

	this->pLight->begin(inpPort, inIntensity);
	this->AddMovingPosition(inId, LIGHTON);
}

void AccessoryLight::Move(unsigned long inId)
{
	if (this->GetMovingPositionSize() == 1)
	{
		this->Toggle();
		return;
	}

	int position = this->IndexOfMovingPositionById(inId);

	if (position != -1)
		this->SetState((ACC_STATE) position);
}

void AccessoryLight::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	if (inEvent == ACCESSORIES_EVENT_MOVEPOSITIONINDEX)
	{
		this->SetLastMovingPosition(inData);
		int oldValue = this->pPort->GetSpeed();
		this->pPort->SetSpeed(this->GetMovingPositionValueByIndex(inData));
		this->LightOn();
		this->pPort->SetSpeed(oldValue);
		return;
	}

	this->pLight->Event(inEvent, inData);
}

#ifndef NO_EEPROM
int AccessoryLight::EEPROMLoad(int inPos)
{
	inPos = this->Accessory::EEPROMLoad(inPos);

	switch (this->GetState())
	{
		case LIGHTON:	this->pPort->MoveLeftDir(); break;
		case LIGHTOFF:	this->pPort->MoveStop(); break;
		case LIGHTBLINK: break;
	}

	return inPos;
}
#endif

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryLight::printAccessory()
{
	Serial.print(F("    Light : ID "));
	Serial.print(this->GetMovingPositionIdByIndex(0));
	Serial.print(F(" / "));
	this->pLight->printAccessory();
}
#endif

#endif
