/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for an accessory list>
*************************************************************/

#include "Accessories.hpp"
#include "ActionsStack.hpp"

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

Accessory *AccessoriesClass::pFirstAccessory = NULL;
AccessoriesClass AccessoriesClass::AccessoriesInstance;

void AccessoriesClass::Add(Accessory *inpAccessory)
{
	if (AccessoriesClass::pFirstAccessory == NULL)
	{
		AccessoriesClass::pFirstAccessory = inpAccessory;
		inpAccessory->SetNextAccessory(NULL);
		return;
	}

	Accessory *pCurr = AccessoriesClass::pFirstAccessory;

	while (pCurr->GetNextAccessory() != NULL)
		pCurr = pCurr->GetNextAccessory();

	pCurr->SetNextAccessory(inpAccessory);
	inpAccessory->SetNextAccessory(NULL);
}

AccessoriesClass::AccessoriesClass()
{
}

void AccessoriesClass::begin()
{
}

Accessory *AccessoriesClass::GetById(unsigned long inId) const
{
	Accessory *pCurr = AccessoriesClass::pFirstAccessory;

	while (pCurr != NULL)
	{
		if (pCurr->IndexOfMovingPosition(inId) != (uint8_t) -1)
			return pCurr;
		pCurr = pCurr->GetNextAccessory();
	}

	return NULL;
}

bool AccessoriesClass::IsActionPending()
{
	Accessory *pCurr = AccessoriesClass::pFirstAccessory;

	while (pCurr != NULL)
	{
		if (pCurr->IsActionPending())
			return true;
		pCurr = pCurr->GetNextAccessory();
	}

	return false;
}

bool AccessoriesClass::CanMove(unsigned long inId)
{
	Accessory *acc = this->GetById(inId);

	if (acc == NULL)
		return false;

	// Move if there is more than one MovingPosition (no toggle id), and moving ids are the same than
	// previous time...
	if (acc->GetMovingPositionSize() > 1)
	{
		bool move = acc->IndexOfMovingPosition(inId) == acc->GetLastMovingPosition();
#ifdef ACCESSORIES_DEBUG_MODE
		if (!move)
			Serial.println(F("Same position : Cant move !"));
#endif
		return move;
	}

	if (millis() - acc->GetLastMoveTime() <= acc->GetDebounceDelay())
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("Debounce : Cant move !"));
#endif
		return false;
	}

	acc->SetLastMoveTime();
	return true;
}

static Accessory *pLoopAccessory = 0;

bool AccessoriesClass::loop()
{
	if (pLoopAccessory == NULL)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("*** Setup Accessories Finished."));
#endif
		pLoopAccessory = AccessoriesClass::pFirstAccessory;
	}
	else
	{
		if (pLoopAccessory->GetNextAccessory() == NULL)
			pLoopAccessory = AccessoriesClass::pFirstAccessory;
		else
			pLoopAccessory = pLoopAccessory->GetNextAccessory();
	}

	if (pLoopAccessory == NULL)
		return false;

	pLoopAccessory->loop();

	// Look for action stack pending...
	if (this->IsActionPending())
		return false;

	Action *act = ActionsStack::Actions.GetActionToExecute();
	if (act != NULL)
		if (this->CanMove(act->Id))
			this->Event(act->Id, act->Event, act->Data);

	return false;
}

bool AccessoriesClass::Toggle(unsigned long inId)
{
	if (ActionsStack::FillingStack)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.print(F(" ---- Stack id "));
		Serial.println(inId);
#endif
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_MOVEPOSITIONID);
		return false;
	}

	Accessory *acc = this->GetById(inId);

	if (acc == NULL)
	{
		return false;
	}

	if (!this->CanMove(inId))
	{
		return true;
	}

	acc->SetLastMovingPosition(inId);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("Toggle : Accessory id "));
	Serial.println(inId);
#endif

	acc->Move(inId);

	return true;
}

bool AccessoriesClass::MovePosition(unsigned long inId)
{
	Accessory *acc = this->GetById(inId);

	if (acc == NULL)
	{
		return false;
	}

	uint8_t pos = acc->IndexOfMovingPosition(inId);

	if (pos == 255)
	{
		return false;
	}

	if (ActionsStack::FillingStack)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.print(F(" ---- Stack id "));
		Serial.println(inId);
#endif
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_MOVEPOSITIONINDEX, pos);
		return false;
	}

	acc->SetLastMovingPosition(pos);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("MovePosition : Accessory id "));
	Serial.print(inId);
	Serial.print(F(" to position "));
	Serial.println(acc->GetMovingPosition(inId));
#endif

	acc->MovePosition(acc->GetMovingPosition(inId));

	return true;
}

void AccessoriesClass::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	if (ActionsStack::FillingStack)
	{
		ActionsStack::Actions.Add(inId, inEvent, inData);
		return;
	}

	Accessory *acc = this->GetById(inId);

	if (acc != NULL)
	{
		if (inEvent == ACCESSORIES_EVENT_MOVEPOSITIONINDEX && (inData < 0 || inData >= acc->GetMovingPositionSize()))
		{
#ifdef ACCESSORIES_DEBUG_MODE
			Serial.print(F("Accessory id "));
			Serial.print(inId);
			Serial.print(F(" bad MovePositionIndex event "));
			Serial.println(inData);
#endif
			return;
		}

		acc->Event(inId, inEvent, inData);
	}
}
