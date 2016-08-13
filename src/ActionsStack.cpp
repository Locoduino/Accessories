/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a actions stack>
*************************************************************/

#include "ActionsStack.hpp"

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

Action::Action(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	this->Id = inId;
	this->Event = inEvent;
	this->Data = inData;
}

ActionsStack ActionsStack::Actions(ACTION_STACK_SIZE);
bool ActionsStack::FillingStack(false);

#ifdef DEBUG_MODE
void ActionsStack::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)	const
{
	if (this->size == 0)
	{
		Serial.print(F("Size undefined in "));
		Serial.println(inFunc);
	}
	else
	if (inIndex < 0 || inIndex >= this->size)
	{
		Serial.print(F("Index error in "));
		Serial.println(inFunc);
	}
}
#endif

ActionsStack::ActionsStack(int inSize)
{
	this->size = inSize;
	this->addCounter = 0;
	this->pList = new Action*[inSize];

	for (int i = 0; i < this->size; i++)
	{
		this->pList[i] = NULL;
	}
}

// Returns the index of the new added action.
unsigned char ActionsStack::Add(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	if (addCounter >= size)
		return size + 1;	// action lost, the stack is full !

	CHECK(addCounter, "ActionsStack::Add event");
	this->pList[addCounter++] = new Action(inId, inEvent, inData);

	return addCounter - 1;
}

Action *ActionsStack::GetActionToExecute()
{
	for (int i = 0; i < this->size; i++)
	{
		if (this->pList[i] != NULL)
		{
			Action *ret = this->pList[i];
			this->pList[i] = NULL;
			return ret;
		}
	}

	return 0;
}

// Returns the index of the new added action.
void ActionsStack::Purge(int inIndex)
{
	if (this->pList[inIndex] != NULL)
	{
		delete this->pList[inIndex];
		this->pList[inIndex] = NULL;
	}
}

// Returns the index of the new added action.
void ActionsStack::Purge()
{
	this->addCounter = 0;

	for (int i = 0; i < this->size; i++)
	{
		delete this->pList[i];
		this->pList[i] = NULL;
	}
}

// Returns the number of stacked actions.
int ActionsStack::GetNumber() const
{
	int count = 0;

	for (int i = 0; i < this->size; i++)
	{
		if (this->pList[i] != NULL)
			count++;
	}

	return count;
}

Action *ActionsStack::operator[](unsigned char inIndex)
{
	CHECK(inIndex, "ActionsStack::operator[]");
	return this->pList[inIndex];
}
