/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a group of accessories>
*************************************************************/

#include "Accessories.h"
#ifndef NO_EEPROM
#include "EEPROM.h"
#endif

#ifndef NO_GROUP
#include "ActionsStack.hpp"

#ifdef VISUALSTUDIO
#include<stdarg.h>
#endif

#ifdef ACCESSORIES_DEBUG_MODE
#define CHECKINDEX(val, text)	CheckIndex(val, F(text))
#define CHECKINDEXITEM(val, text)	CheckIndex(val, F(text))
#else
#define CHECKINDEX(val, text)
#define CHECKINDEXITEM(val, text)
#endif

/***********************************************************
*		GroupStateItem
************************************************************/

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void GroupStateItem::printAccessory()
{
	Serial.print(F("             Acc : ID "));
	Serial.print(this->pAccessory->GetMovingPositionIdByIndex(0));
	Serial.print(F(" / State "));
	Serial.print(this->State);
	Serial.print(F(" / AssyncDelay "));
	Serial.println(this->Delay);
}
#endif

/***********************************************************
*		GroupState
************************************************************/

GroupState::GroupState(unsigned long inId, bool inSynchrone)
{
	this->Id = inId;
	this->Synchronous = inSynchrone;
	this->Items.HasCurrentNull = true;
}

void GroupState::begin()
{
#ifdef ACCESSORIES_DEBUG_MODE
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
	if (this->IsActionItemPending())
	{
		this->Items.ResetCurrent();
		return;
	}

	this->startActionTime = 0;

	if (this->Synchronous)
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
	if (!this->IsActionItemPending())
		return;

	if (this->Items.pCurrentItem->Obj->pAccessory->IsGroupActionPending())
		return;

	if (this->startActionTime != 0)
		if (millis() - this->startActionTime < this->Items.pCurrentItem->Obj->Delay)
			return;

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryGroupState next action state accessory "));
#endif

	this->Items.NextCurrent();

	if (this->Items.HasCurrent())
		this->Items.pCurrentItem->Obj->pAccessory->StartAction(this->Items.pCurrentItem->Obj->State);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void GroupState::printAccessory()
{
	Serial.print(F("        State : ID "));
	Serial.println(this->Id);

	ACCSCHAINEDLISTITEM<GroupStateItem> *pCurr = this->Items.pFirst;

	while (pCurr != NULL)
	{
		pCurr->Obj->printAccessory();
		pCurr = pCurr->pNext;
	}
}
#endif

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
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("   AccessoryGroup begin"));
#endif
}

void AccessoryGroup::AddStateItem(unsigned long inId, Accessory &inAccessory, ACC_STATE inState, unsigned int inDelay)
{
	GroupState *pState = this->GetByID(inId);

	if (pState == NULL)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("   AccessoryGroup wrong state id !"));
#endif
		return;
	}

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

uint8_t AccessoryGroup::GetCount()
{
	uint8_t count = 0;
	AccessoryGroup *pCurr = AccessoryGroup::pFirstGroup;
	while (pCurr != NULL)
	{
		count++;
		pCurr = pCurr->GetNextGroup();
	}

	return count;
}

GroupState *AccessoryGroup::GetByID(unsigned long inId) const
{
	ACCSCHAINEDLISTITEM<GroupState> *pItem = this->GetItemByID(inId);

	if (pItem == NULL)
		return NULL;

	return pItem->Obj;
}

ACCSCHAINEDLISTITEM<GroupState> *AccessoryGroup::GetItemByID(unsigned long inId) const
{
	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		if (pCurr->Obj->GetId() == inId)
			return pCurr;
		pCurr = pCurr->pNext;
	}

	return NULL;
}

bool AccessoryGroup::IsActionItemPending()
{
	return (this->States.HasCurrent());
}

void AccessoryGroup::StartAction(GroupState *inpState)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryGroup start action state "));
	Serial.println(inpState->GetId());
#endif

	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		if (pCurr->Obj == inpState)
		{
			this->States.pCurrentItem = pCurr;
#ifndef NO_EEPROM
			Accessories::EEPROMSave();
#endif
			pCurr->Obj->StartAction();
			return;
		}
		pCurr = pCurr->pNext;
	}
}

bool AccessoryGroup::loop()
{
	if (!this->IsActionItemPending())
		return false;	// nothing done !

	this->States.pCurrentItem->Obj->loop();
	if (!this->States.pCurrentItem->Obj->IsActionItemPending())
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
			if (pCurrState->Obj->GetId() == inId)
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
		if (pCurr->Obj->GetId() == inId)
			break;
		pCurr = pCurr->pNext;
	}

	if (pCurr == NULL)
		return false;

	if (ActionsStack::FillingStack)
	{
		ActionsStack::Actions.Add(inId, ACCESSORIES_EVENT_MOVEPOSITIONID);
		return true;
	}

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryGroup moved "));
	Serial.println(inId);
#endif
	if (!this->IsActionItemPending())
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("No action item pending..."));
#endif
		this->States.pCurrentItem = pCurr;
#ifndef NO_EEPROM
		Accessories::EEPROMSave();
#endif
		pCurr->Obj->StartAction();
	}

	return true;
}

#ifndef NO_EEPROM
int AccessoryGroup::EEPROMSave(int inPos, bool inSimulate)
{
	if (!inSimulate)
	{
		if (this->States.pCurrentItem != NULL)
		{
			unsigned long id = this->States.pCurrentItem->Obj->GetId();
			byte *pEvent = (byte *)&id;
			EEPROM.write(inPos, *pEvent);
			EEPROM.write(inPos+1, *(pEvent + 1));
			EEPROM.write(inPos+2, *(pEvent + 2));
			EEPROM.write(inPos+3, *(pEvent + 3));
		}
		else
		{
			EEPROM.write(inPos, 0);
			EEPROM.write(inPos+1, 0);
			EEPROM.write(inPos+2, 0);
			EEPROM.write(inPos+3, 0);
		}
	}

	inPos += sizeof(unsigned long);

	return inPos;
}

int AccessoryGroup::EEPROMLoad(int inPos)
{
	long four = EEPROM.read(inPos++);
	long three = EEPROM.read(inPos++);
	long two = EEPROM.read(inPos++);
	long one = EEPROM.read(inPos++);

	if (one+two+three+four > 0)
	{
		//Rebuild the recomposed long by using bitshift.
		unsigned long ID = ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);

		ACCSCHAINEDLISTITEM<GroupState> *grp = AccessoryGroup::GetItemByID(ID);
		this->States.pCurrentItem = grp;
	}
	else
	{
		this->States.pCurrentItem = NULL;
	}

	return inPos;
}

int AccessoryGroup::EEPROMSaveAll(int inPos, bool inSimulate)
{
	AccessoryGroup *pCurr = pFirstGroup;

	while (pCurr != NULL)
	{
		inPos = pCurr->EEPROMSave(inPos, inSimulate);
		pCurr = pCurr->pNextGroup;
	}

	return inPos;
}

int AccessoryGroup::EEPROMLoadAll(int inPos)
{
	AccessoryGroup *pCurr = pFirstGroup;

	while (pCurr != NULL)
	{
		inPos = pCurr->EEPROMLoad(inPos);
		pCurr = pCurr->pNextGroup;
	}

	return inPos;
}
#endif

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryGroup::printGroups()
{
	AccessoryGroup *pCurr = pFirstGroup;

	while (pCurr != NULL)
	{
		pCurr->printAccessory();
		pCurr = pCurr->pNextGroup;
	}
}

void AccessoryGroup::printAccessory()
{
	Serial.println(F("    Group : "));

	ACCSCHAINEDLISTITEM<GroupState> *pCurr = this->States.pFirst;

	while (pCurr != NULL)
	{
		pCurr->Obj->printAccessory();
		pCurr = pCurr->pNext;
	}
}
#endif
#endif
