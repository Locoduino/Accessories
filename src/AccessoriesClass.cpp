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
#ifdef DEBUG_MODE
		// Done by the SerialCommander setup...
		Serial.begin(115200);
		// Just for let the time to the PIC to initialize internals...
		delay(500);

		Serial.println(F(""));
		Serial.println(F("Accessories V0.16"));
		Serial.println(F("Developed by Thierry Paris."));
		Serial.println(F("(c) Locoduino 2016"));
		Serial.println(F(""));

		Serial.println(F("*** Setup Accessories started."));
#endif

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
		if (pCurr->IndexOfMovingPosition(inId) != -1)
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
		bool move = acc->GetLastMovingPosition().Id != inId;
#ifdef DEBUG_MODE
		if (!move)
			Serial.println(F("Same position : Cant move !"));
#endif
		return move;
	}

	if (millis() - acc->GetLastMoveTime() <= acc->GetDebounceDelay())
	{
#ifdef DEBUG_MODE
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
#ifdef DEBUG_MODE
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
#ifdef DEBUG_MODE
		Serial.print(F(" ---- Stack id "));
		Serial.println(inId);
#endif
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_TOGGLE);
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

#ifdef DEBUG_MODE
	Serial.print(F("Toggle : Accessory id "));
	Serial.println(inId);
#endif

	acc->Move(inId);

	return true;
}

bool AccessoriesClass::MovePosition(unsigned long inId, int inPosition)
{
	if (ActionsStack::FillingStack)
	{
#ifdef DEBUG_MODE
		Serial.print(F(" ---- Stack id "));
		Serial.println(inId);
#endif
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_MOVEPOSITION, inPosition);
		return false;
	}

	Accessory *acc = this->GetById(inId);

	if (acc == NULL)
	{
		return false;
	}

	acc->SetLastMovingPosition(inId, inPosition);

#ifdef DEBUG_MODE
	Serial.print(F("MovePosition : Accessory id "));
	Serial.print(inId);
	Serial.print(F(" to position "));
	Serial.println(inPosition);
#endif

	acc->MovePosition(inPosition);

	return true;
}

void AccessoriesClass::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	Accessory *acc = this->GetById(inId);

	if (acc != NULL)
		acc->Event(inId, inEvent, inData);
}
