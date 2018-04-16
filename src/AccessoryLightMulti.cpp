/**********************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a light, flashing or not, with optional fading>
***********************************************************************/

#include "Accessories.h"
#include "AccessoryLightMulti.hpp"

#ifndef NO_LIGHT

AccessoryLightMulti::AccessoryLightMulti()
{
	this->lightsSize = 0;
	this->pLights = NULL;
	this->pMovingPositionBlinks = NULL;
}

#ifdef ACCESSORIES_DEBUG_MODE
void AccessoryLightMulti::CheckPort() const
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
	{
		if (this->pLights[i].GetPort() == NULL)
		{
			Serial.println(F("One light in a multilight accessory have no port !"));
		}
	}
}
#endif

void AccessoryLightMulti::begin(unsigned long inId, uint8_t inSize, unsigned long inBlinkDuration)
{
	this->lightsSize = inSize;

	this->pLights = new AccessoryBaseLight[inSize];
	this->SetDuration(inBlinkDuration);

	for (uint8_t i = 0; i < inSize; i++)
	{
		this->pLights[i].pOwner = this;
		// This is the default value for the blinking delay, which can be overwritten by individual SetBlinking()
		// on a particular light.
		this->pLights[i].SetBlinking(inBlinkDuration);
	}

	this->pMovingPositionBlinks = NULL;
}

void AccessoryLightMulti::AdjustMovingPositionBlinksSize(uint8_t inNewSize)
{
	if (this->pMovingPositionBlinks == NULL || inNewSize > this->GetMovingPositionSize())
	{
		int i = 0;
		int *pNewMovingPositionBlinks = new int[inNewSize];
		if (pMovingPositionBlinks != NULL)
			for (i = 0; i < this->GetMovingPositionSize(); i++)
				pNewMovingPositionBlinks[i] = this->pMovingPositionBlinks[i];

		for (; i < inNewSize; i++)
			pNewMovingPositionBlinks[i] = 0;

		if (this->pMovingPositionBlinks != NULL)
			delete[] this->pMovingPositionBlinks;
		this->pMovingPositionBlinks = pNewMovingPositionBlinks;
	}
}

unsigned char AccessoryLightMulti::AddMovingPosition(unsigned long inIdMin, int inOnMask, int inBlinkMask)
{
	unsigned char pos = Accessory::AddMovingPosition(inIdMin, inOnMask);
	
	this->AdjustMovingPositionBlinksSize(this->GetMovingPositionSize());
	this->pMovingPositionBlinks[pos] = inBlinkMask;

	return pos;
}

void AccessoryLightMulti::beginLight(uint8_t inIndex, Port *inpPort, int inIntensity)
{
	this->pLights[inIndex].begin(inpPort, inIntensity, this);
	this->LightOff(inIndex);
}

void AccessoryLightMulti::LightOn()
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
		this->LightOn(i);
}

void AccessoryLightMulti::LightOff()
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
		this->LightOff(i);
}

void AccessoryLightMulti::Blink()
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
		this->Blink(i);
}

bool AccessoryLightMulti::ActionEnded()
{
	bool res = false;
	for (uint8_t i = 0; i < this->lightsSize; i++)
		res |= this->ActionEnded(i);

	return res;
}

ACC_STATE AccessoryLightMulti::Toggle()
{
	ACC_STATE localState = ACC_STATE::STATE_NONE;

	for (uint8_t i = 0; i < this->lightsSize; i++)
		localState = this->Toggle(i);

	return localState;
}

void AccessoryLightMulti::Move(int inPosition)
{
	if (inPosition != -1)
	{
		for (uint8_t i = 0; i < this->lightsSize; i++)
			if (inPosition & (1 << i))
				this->SetState(i, LIGHTON);
			else
				this->SetState(i, LIGHTOFF);
	}
}

void AccessoryLightMulti::MoveBlink(int inOnMask, int inBlinkMask)
{
	if (inOnMask != -1)
	{
		for (uint8_t i = 0; i < this->lightsSize; i++)
		{
			this->SetBlinking(i, 0);
			if (inOnMask & (1 << i))
			{
				if (inBlinkMask & (1 << i))
				{
					this->SetBlinking(i, this->GetDuration());
					this->SetState(i, LIGHTBLINK);
				}
				else
					this->SetState(i, LIGHTON);
			}
			else
				this->SetState(i, LIGHTOFF);
		}
	}
}

void AccessoryLightMulti::Move(unsigned long inId)
{
	int positionIndex = this->IndexOfMovingPositionById(inId);

	if (positionIndex != -1)
	{
		int position = this->GetMovingPositionValueById(inId);
		if (this->pMovingPositionBlinks != NULL)
			MoveBlink(position, this->pMovingPositionBlinks[positionIndex]);
		else
			Move(position);
	}
	else
	{
		for (uint8_t i = 0; i < this->lightsSize; i++)
			this->Toggle(i);
	}
}

void AccessoryLightMulti::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData) 
{ 
	int positionIndex = -1;
	if (inEvent == ACCESSORIES_EVENT_MOVEPOSITIONID)
		positionIndex = this->IndexOfMovingPositionById(inId);
	if (inEvent == ACCESSORIES_EVENT_MOVEPOSITIONINDEX)
		positionIndex = inData;

	if (positionIndex > -1)
	{
		this->SetLastMovingPosition(positionIndex);

		if (this->pMovingPositionBlinks != NULL)
			this->MoveBlink(this->GetMovingPositionValueByIndex(positionIndex), this->pMovingPositionBlinks[positionIndex]);
		else
			this->Move(this->GetMovingPositionValueByIndex(positionIndex));
		return;
	}

	switch (inEvent)
	{
	case ACCESSORIES_EVENT_TOGGLE:
		this->Toggle();
		break;

	case ACCESSORIES_EVENT_MOVE:
		switch (inData)
		{
		case ACCESSORIES_MOVE_STRAIGHT:
		case ACCESSORIES_MOVE_TOP:
		case ACCESSORIES_MOVE_LEFT:
		case ACCESSORIES_MOVE_DIVERGE:
		case ACCESSORIES_MOVE_BOTTOM:
		case ACCESSORIES_MOVE_RIGHT:
		case ACCESSORIES_MOVE_ON:
			this->LightOn();
			break;
		case ACCESSORIES_MOVE_OFF:
		case ACCESSORIES_MOVE_STOP:
			this->LightOff();
			break;
		case ACCESSORIES_MOVE_MORE:
		case ACCESSORIES_MOVE_LESS:
		{
			for (uint8_t i = 0; i < this->lightsSize; i++) 
			{
				int oldValue = this->pLights[i].pPort->GetSpeed();
				this->pLights[i].pPort->SetSpeed(oldValue + inData);
				this->LightOn(i);
				this->pLights[i].pPort->SetSpeed(oldValue);
			}
		}
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
	{
		for (uint8_t i = 0; i < this->lightsSize; i++)
		{
			int oldValue = this->pLights[i].pPort->GetSpeed();
			this->pLights[i].pPort->SetSpeed(inData);
			this->LightOn(i);
			this->pLights[i].pPort->SetSpeed(oldValue);
		}
	}
	break;

	case ACCESSORIES_EVENT_SETSPEED:
		for (uint8_t i = 0; i < this->lightsSize; i++)
			this->pLights[i].pPort->SetSpeed(inData);
		break;

	default:
		break;
	}
}

#ifndef NO_EEPROM
int AccessoryLightMulti::EEPROMSave(int inPos, bool inSimulate)
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
		inPos = this->pLights[i].EEPROMSave(inPos, inSimulate);

	return inPos;
}

int AccessoryLightMulti::EEPROMLoad(int inPos)
{
	for (uint8_t i = 0; i < this->lightsSize; i++)
		inPos = this->pLights[i].EEPROMLoad(inPos);

	return inPos;
}
#endif
#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryLightMulti::printAccessory()
{
	Serial.println(F("    LightMulti"));
	for (uint8_t i = 0; i < this->lightsSize; i++)
	{
		Serial.print(F("         Light "));
		Serial.print(i);
		Serial.print(F(" : "));
		this->pLights[i].printAccessory();
	}
	this->printMovingPositions();
}

void AccessoryLightMulti::printMovingPositions()
{
	for (int i = 0; i < this->GetMovingPositionSize(); i++)
	{
		Serial.print(F("         MovingPosition "));
		Serial.print(i);
		Serial.print(F(": id "));
		Serial.print(this->GetMovingPositionIdByIndex(i));
		Serial.print(F(" / pos "));
		Serial.print(this->GetMovingPositionValueByIndex(i), BIN);
		if (this->pMovingPositionBlinks != NULL)
		{
			Serial.print(F(" / blink "));
			Serial.println(this->pMovingPositionBlinks[i], BIN);
		}
		else
			Serial.println("");
	}
}
#endif

#endif
