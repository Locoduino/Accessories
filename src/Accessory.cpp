/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a generic accessory>
*************************************************************/

#include "Accessories.h"

Accessory::Accessory()
{
	this->pPort = NULL;
	this->movingPositionsSize = 0;
	this->movingPositionsAddCounter = 0;
	this->pMovingPositions = NULL;
	this->lastMovingPosition.Id = UNDEFINED_ID;
	this->debounceDelay = 300;
	this->SetLastMoveTime();

	this->duration = 0;
	this->startingMillis = 0;
	this->useStateNone = STATE_NONE;
	this->type = ACCESSORYUNDEFINED;
	AccessoriesClass::AccessoriesInstance.Add(this);
}

void Accessory::AdjustMovingPositionsSize(int inNewSize)
{
	if (inNewSize <= this->movingPositionsSize)
		return;

	MovingPosition *pNewList = new MovingPosition[inNewSize];
	int i = 0;
	for (; i < this->movingPositionsSize; i++)
		pNewList[i] = this->pMovingPositions[i];

	for (; i < inNewSize; i++)
		pNewList[i].Id = UNDEFINED_ID;	//empty

	this->movingPositionsSize = inNewSize;
	if (this->pMovingPositions != 0)
		delete[] this->pMovingPositions;
	this->pMovingPositions = pNewList;
}

// Returns the index of the new added position.
unsigned char Accessory::AddMovingPosition(unsigned long inId, int inPosition)
{
	Accessory::AdjustMovingPositionsSize(this->movingPositionsAddCounter + 1);

	this->pMovingPositions[this->movingPositionsAddCounter].Id = inId;
	this->pMovingPositions[this->movingPositionsAddCounter++].Position = inPosition;

	return this->movingPositionsAddCounter - 1;
}

int Accessory::IndexOfMovingPosition(unsigned long inId) const
{
	for (int i = 0; i < this->movingPositionsSize; i++)
		if (this->pMovingPositions[i].Id == inId)
			return i;

	return -1;
}

int Accessory::GetMovingPosition(unsigned long inId) const
{
	for (int i = 0; i < this->movingPositionsSize; i++)
		if (this->pMovingPositions[i].Id == inId)
			return this->pMovingPositions[i].Position;

	return UNDEFINED_POS;
}

void Accessory::StartAction()
{
	if (this->duration > 0)
		this->startingMillis = millis();

#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("Accessory start action "));
	Serial.println(this->startingMillis);
#endif
#endif
}

void Accessory::StartAction(ACC_STATE inState)
{
	if (this->duration > 0)
		this->startingMillis = millis();
	this->SetState(inState);

#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("Accessory start action at "));
	Serial.print(this->startingMillis);
	Serial.print(F("ms for state "));
	Serial.print(inState);

	if (this->startingMillis == 0)
		Serial.println(" ended");
	else
		Serial.println("");
#endif
#endif
}

#ifdef DEBUG_VERBOSE_MODE
void Accessory::ResetAction()
{
	Serial.print(F("End (reset) action at "));
	Serial.print(millis() - this->startingMillis);
	Serial.print(F("ms for "));
	Serial.print(this->duration);
	Serial.println(F("ms"));

	this->startingMillis = 0;
}
#endif

bool Accessory::ActionEnded()
{
	if (this->startingMillis <= 0)
		return false;

	if ((unsigned long)(millis() - this->startingMillis) > this->duration)
	{
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
		Serial.print(F("End action at "));
		Serial.print(millis() - this->startingMillis);
		Serial.print(F("ms for "));
		Serial.print(this->duration);
		Serial.println(F("ms"));
#endif
#endif
		this->startingMillis = 0;
		return true;
	}

	return false;
}


