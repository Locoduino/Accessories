//-------------------------------------------------------------------
#ifndef __actionsStack_H__
#define __actionsStack_H__
//-------------------------------------------------------------------

#include "Accessories.h"

//-------------------------------------------------------------------

// An action is what can be done by the user to ask for a accessory movement.
// It can be a move, a stop... Other kind of actions can be added in EVENT_TYPE if necessary.

class Action
{
	public:
		unsigned long Id;
		ACCESSORIES_EVENT_TYPE Event;
		int Data;

		Action(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData = 0);
};

// This is a FIFO stack, grabbing all user actions to be done later. When the stack reach the size,
// other actions are lost...

class ActionsStack
{
	private:
		unsigned char size;
		unsigned char addCounter;
		Action* *pList;
		
	public:
		ActionsStack(int inSize);
		
		unsigned char Add(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData = 0);
		Action *operator[](unsigned char idx);
		void Purge();
		void Purge(int inIndex);
		Action *GetActionToExecute();
		int GetNumber() const;

		static ActionsStack Actions;
		static bool FillingStack;

	private:
		void CheckIndex(unsigned char inIndex, const __FlashStringHelper *infunc) const;
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
