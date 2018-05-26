//-------------------------------------------------------------------
#ifndef __actionsStack_H__
#define __actionsStack_H__
//-------------------------------------------------------------------

#include "Accessories.h"

/**This class is an item of the ActionStack.	
An action is what can be done by the user to ask for a accessory movement.
It can be a move, a stop... Other kind of actions can be added in ACCESSORIES_EVENT_TYPE if necessary.*/
class Action
{
	public:
		/** Id of the accessory.*/
		unsigned long Id;
		/** Event type.*/
		ACCESSORIES_EVENT_TYPE Event;
		/** Associated data.*/
		int Data;

		/** Default constructor
		@param inId	Id of the accessory
		@param inEvent	Event type
		@param inData	Associated data for this event. Default is 0.
		*/
		Action(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData = 0);
};

/**This class is a small list of available actions.
When the stack reach the size, new actions are lost...*/ 
class ActionsStack
{
	private:
		unsigned char size;
		Action* *pList;
		
	public:
		/** Default constructor.*/
		ActionsStack(int inSize);
		
		/**Add a new action.
		@param inId	Id of the accessory
		@param inEvent	Event type
		@param inData	Associated data for this event. Default is 0.
		@return Index of the new action or 'size of the stack+1' if the stack is full.
		@remark If the stack is full, this event will be lost.
		*/
		unsigned char Add(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData = 0);
		/**Array operator.
		@param index index of the action to get.
		*/
		inline Action *operator[](unsigned char index) { return this->pList[index]; }
		/** Clears the complete stack.*/
		void Clear();
		/** Remove the given index of the stack.
		@param inIndex index to clear.
		*/
		void Delete(int inIndex);
		/**Get an action to execute.
		@return action index or 255
		*/
		unsigned char GetActionToExecute();
		/**Get the number of available actions to execute.
		@return action number.
		*/
		int GetNumber() const;

		/**Static list of actions.*/
		static ActionsStack Actions;
		/**If this flag is false, the event are not added t the list.*/
		static bool FillingStack;
};

#endif
