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

void AccessoryLightMulti::begin(unsigned long inId, byte inSize, unsigned long inBlinkDuration)
{
	this->lightsSize = inSize;

	this->pLights = new AccessoryBaseLight[inSize];

	for (byte i = 0; i < inSize; i++)
	{
		this->pLights[i].pOwner = this;
		this->pLights[i].SetBlinking(inBlinkDuration);
	}

	this->pMovingPositionBlinks = NULL;
}

unsigned char AccessoryLightMulti::AddMovingPosition(unsigned long inIdMin, int inOnMask, int inBlinkMask)
{
	unsigned char pos = Accessory::AddMovingPosition(inIdMin, inOnMask);

	if (this->pMovingPositionBlinks == NULL)
	{
		this->pMovingPositionBlinks = new int[this->GetMovingPositionSize()];
		for (int i = 0; i < this->GetMovingPositionSize(); i++)
			this->pMovingPositionBlinks[i] = 0;
	}

	this->pMovingPositionBlinks[pos] = inBlinkMask;
	return 0;
}

void AccessoryLightMulti::beginLight(byte inIndex, DriverPort *inpPort, int inIntensity)
{
	this->pLights[inIndex].begin(inpPort, inIntensity, this);
	this->LightOff(inIndex);
}

void AccessoryLightMulti::LightOn()
{
	for (byte i = 0; i < this->lightsSize; i++)
		this->LightOn(i);
}

void AccessoryLightMulti::LightOff()
{
	for (byte i = 0; i < this->lightsSize; i++)
		this->LightOff(i);
}

void AccessoryLightMulti::Blink()
{
	for (byte i = 0; i < this->lightsSize; i++)
		this->Blink(i);
}

bool AccessoryLightMulti::ActionEnded()
{
	bool res = false;
	for (byte i = 0; i < this->lightsSize; i++)
		res |= this->ActionEnded(i);

	return res;
}

ACC_STATE AccessoryLightMulti::Toggle()
{
	ACC_STATE localState = ACC_STATE::STATE_NONE;

	for (byte i = 0; i < this->lightsSize; i++)
		localState = this->Toggle(i);

	return localState;
}

void AccessoryLightMulti::Move(int inPosition)
{
	if (inPosition != -1)
	{
		for (byte i = 0; i < this->lightsSize; i++)
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
		for (byte i = 0; i < this->lightsSize; i++)
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
	int positionIndex = this->IndexOfMovingPosition(inId);

	if (positionIndex != -1)
	{
		int position = this->GetMovingPosition(positionIndex).Position;
		if (this->pMovingPositionBlinks != NULL)
			MoveBlink(position, this->pMovingPositionBlinks[positionIndex]);
		else
			Move(position);
	}
	else
	{
		for (byte i = 0; i < this->lightsSize; i++)
			this->Toggle(i);
	}
}

#endif
