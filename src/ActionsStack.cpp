/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a actions stack>
*************************************************************/

#include "ActionsStack.hpp"

Action::Action(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	this->Id = inId;
	this->Event = inEvent;
	this->Data = inData;
}

ActionsStack ActionsStack::Actions(ACTION_STACK_SIZE);
bool ActionsStack::FillingStack(false);

ActionsStack::ActionsStack(int inSize)
{
	this->size = inSize;
	this->pList = new Action*[inSize];

	for (int i = 0; i < this->size; i++)
	{
		this->pList[i] = NULL;
	}
}

// Returns the index of the new added action.
unsigned char ActionsStack::Add(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	for (int i = 0; i < this->size; i++)
	{
		if (this->pList[i] == NULL)
		{
			this->pList[i] = new Action(inId, inEvent, inData);
			return i;
		}
	}

	return this->size + 1;	// action lost, the stack is full !
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

	return NULL;
}

// Returns the index of the new added action.
void ActionsStack::Delete(int inIndex)
{
	if (this->pList[inIndex] != NULL)
	{
		delete this->pList[inIndex];
		this->pList[inIndex] = NULL;
	}
}

// Returns the index of the new added action.
void ActionsStack::Clear()
{
	for (int i = 0; i < this->size; i++)
		this->Delete(i);
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
	return this->pList[inIndex];
}
