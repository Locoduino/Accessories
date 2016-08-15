/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a group of accessories>
*************************************************************/

#include "Accessories.h"

#ifndef NO_GROUP
#include "ActionsStack.hpp"

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

#ifdef DEBUG_MODE
#define CHECKINDEX(val, text)	CheckIndex(val, F(text))
#define CHECKINDEXITEM(val, text)	CheckIndex(val, F(text))
#else
#define CHECKINDEX(val, text)
#define CHECKINDEXITEM(val, text)
#endif

/***********************************************************
*		GroupState
************************************************************/

GroupState::GroupState(unsigned long inId, bool inSynchrone)
{
	this->Id = inId;
	this->Synchrone = inSynchrone;
	this->Items.HasCurrentNull = true;
}

void GroupState::begin()
{
#ifdef DEBUG_MODE
	Serial.println(F("   GroupState begin"));
#endif
}

/*void GroupState::begin(GroupStateItem *inpFirstItem, ...)
{
	va_list argList;
	this->Items.AddItem(*inpFirstItem);

	va_start(argList, inpFirstItem);
	GroupStateItem *item = NULL;
	do {
		if (item != NULL)
			this->Items.AddItem(*item);
		item = va_arg(argList, GroupStateItem *);
	} while (item != NULL);
	va_end(argList);
} */

void GroupState::Add(Accessory *inpAccessory, ACC_STATE inState, unsigned int inDelay)
{ 
	this->Items.AddItem(new GroupStateItem(inpAccessory, inState, inDelay)); 
}

void GroupState::StartAction()
{
	if (IsActionPending())
	{
		this->Items.ResetCurrent();
		return;
	}

	this->startActionTime = 0;

	if (this->Synchrone)
	{
		ACCSCHAINEDLISTITEM<GroupStateItem> *pCurr = this->Items.pFirst;

		while (pCurr != NULL)
		{
			pCurr->Obj->pAccessory->StartAction(pCurr->Obj->State);
			pCurr = pCurr->pNext;
		}

		this->Items.ResetCurrent();
		return;
	}

	this->Items.StartCurrent();
	if (this->Items.pCurrentItem->Obj->Delay != 0)
		this->startActionTime = millis();

	this->Items.pCurrentItem->Obj->pAccessory->StartAction(this->Items.pCurrentItem->Obj->State);
}

void GroupState::loop()
{
	if (!IsActionPending())
		return;

	if (this->Items.pCurrentItem->Obj->pAccessory->IsGroupActionPending())
		return;

	if (this->startActionTime != 0)
		if (millis() - this->startActionTime < this->Items.pCurrentItem->Obj->Delay)
			return;

#ifdef DEBUG_MODE
	Serial.print(F("AccessoryGroupState next action state accessory "));
#endif

	this->Items.NextCurrent();

	if (this->Items.HasCurrent())
		this->Items.pCurrentItem->Obj->pAccessory->StartAction(this->Items.pCurrentItem->Obj->State);
}

/***********************************************************
*		AccessoryGroup
************************************************************/

AccessoryGroup *AccessoryGroup::pFirstGroup = NULL;

AccessoryGroup::AccessoryGroup()
{
	this->States.HasCurrentNull = true;
	this->pNextGroup = NULL;

	AddGroup(this);
}

void AccessoryGroup::begin()
{
#ifdef DEBUG_MODE
	Serial.println(F("   AccessoryGroup begin"));
#endif
}

void AccessoryGroup::AddStateItem(unsigned long inId, Accessory &inAccessory, ACC_STATE inState, unsigned int inDelay)
{
	GroupState *pState = this->GetByID(inId);

	if (pState == NULL)
		return;

	pState->Add(&inAccessory, inState, inDelay);
}

void AccessoryGroup::AddGroup(AccessoryGroup *inGroup)
{
	if (AccessoryGroup::pFirstGroup == NULL)
	{
		AccessoryGroup::pFirstGroup = inGroup;
		inGroup->SetNextGroup(NULL);
		return;
	}

	AccessoryGroup *pCurr = AccessoryGroup::pFirstGroup;

	while (pCurr->GetNextGroup() != NULL)
		pCurr = pCurr->GetNextGroup();

	pCurr->SetNextGroup(inGroup);
	inGroup->SetNextGroup(NULL);
}

void AccessoryGroup::AddRange(const AccessoryGroup &inGroup)
{
	ACCSCHAINEDLISTITEM<GroupState> *pCurr = inGroup.States.pFirst;

	while (pCurr != NULL)
	{
		this->States.AddItem(pCurr->Obj);
		pCurr = pCurr->pNext;
	}
}

GroupState *AccessoryGroup::GetByID(unsigned long inId)
{
	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		if(pCurr->Obj->Id == inId)
			return pCurr->Obj;
		pCurr = pCurr->pNext;
	}

	return NULL;
}

bool AccessoryGroup::IsActionPending()
{
	return (this->States.HasCurrent());
}

void AccessoryGroup::StartAction(GroupState *inpState)
{
#ifdef DEBUG_MODE
	Serial.print(F("AccessoryGroup start action state "));
	Serial.println(inpState->Id);
#endif

	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		if (pCurr->Obj == inpState)
		{
			this->States.pCurrentItem = pCurr;
			pCurr->Obj->StartAction();
			return;
		}
		pCurr = pCurr->pNext;
	}
}

bool AccessoryGroup::loop()
{
	if (!IsActionPending())
		return false;	// nothing done !

	this->States.pCurrentItem->Obj->loop();
	if (!this->States.pCurrentItem->Obj->IsActionPending())
		this->States.ResetCurrent();
	return true;
}

static AccessoryGroup *pLoopGroup = NULL;

bool AccessoryGroup::loops()
{
	if (pLoopGroup == NULL)
	{
		pLoopGroup = AccessoryGroup::pFirstGroup;
	}
	else
	{
		if (pLoopGroup->GetNextGroup() == NULL)
			pLoopGroup = AccessoryGroup::pFirstGroup;
		else
			pLoopGroup = pLoopGroup->GetNextGroup();
	}

	if (pLoopGroup == NULL)
		return false;

	return pLoopGroup->loop();
}

void AccessoryGroup::EventAll(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	AccessoryGroup *pCurr = AccessoryGroup::pFirstGroup;

	while (pCurr != NULL)
	{
		ACCSCHAINEDLISTITEM<GroupState> *pCurrState = pCurr->States.pFirst;

		while (pCurrState != NULL)
		{
			if (pCurrState->Obj->Id == inId)
				break;
			pCurrState = pCurrState->pNext;
		}

		if (pCurrState != NULL)
			break;

		pCurr = pCurr->GetNextGroup();
	}

	if (pCurr != NULL)
		pCurr->Event(inId, inEvent, inData);
}

void AccessoryGroup::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	this->Toggle(inId);
}

bool AccessoryGroup::Toggle(unsigned long inId)
{
	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		if (pCurr->Obj->Id == inId)
			break;
		pCurr = pCurr->pNext;
	}

	if (pCurr == NULL)
		return false;

	if (ActionsStack::FillingStack)
	{
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_TOGGLE);
		return true;
	}

#ifdef DEBUG_MODE
	Serial.print(F("AccessoryGroup moved "));
	Serial.println(inId);
#endif
	if (!this->IsActionPending())
	{
#ifdef DEBUG_MODE
		Serial.println(F("No action pending..."));
#endif
		this->States.pCurrentItem = pCurr;
		pCurr->Obj->StartAction();
	}

	return true;
}
#endif
