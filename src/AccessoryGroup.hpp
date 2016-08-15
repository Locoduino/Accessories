//-------------------------------------------------------------------
#ifndef __accessoryGroup_H__
#define __accessoryGroup_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
#include "Chain.hpp"

//-------------------------------------------------------------------

#ifndef NO_GROUP

// One item : Motor1/left or Light18 -/On
// The delay gives a duration of the movement, to ensure that this movement is finished before starting the next one. Only used in asynchronous group state.
class GroupStateItem
{
	public:
		Accessory *pAccessory;
		ACC_STATE State;
		unsigned int Delay;

		GroupStateItem() { this->pAccessory = NULL; this->State = STATE_NONE; this->Delay = 0; }
		GroupStateItem(Accessory *inpAccessory, ACC_STATE inState, unsigned int inDelay = 0) { this->pAccessory = inpAccessory; this->State = inState; this->Delay = inDelay; }
};

// A complete state of the group : Light18/On and Motor3/Right .
class GroupState
{
	public:
		unsigned long Id;
		ACCSCHAINEDLIST<GroupStateItem> Items;
		bool Synchrone;

	private:
		unsigned long startActionTime;

	public:
		GroupState() { this->Id = UNDEFINED_ID; this->Synchrone = false; }
		GroupState(unsigned long inId, bool inSynchrone = true);

		//void begin(GroupStateItem *inpFirstItem, ...);
		void begin();
	
		void StartAction();
		void ResetAction()	{ this->Items.ResetCurrent(); }
		bool IsActionPending()	{ return this->Items.HasCurrent(); }

		void loop();
	public:
		void Add(Accessory *inpAccessory, ACC_STATE inState, unsigned int inDelay = 0);
};

// Group of accessories defining a list of states of these acessories.
class AccessoryGroup
{
	public:
		static AccessoryGroup *pFirstGroup;
		inline void SetNextGroup(AccessoryGroup *inAccessory) { this->pNextGroup = inAccessory; }
		inline AccessoryGroup *GetNextGroup() const { return this->pNextGroup; }
		static void AddGroup(AccessoryGroup *inGroup);
		static bool loops();

	public:
		AccessoryGroup *pNextGroup;
		ACCSCHAINEDLIST<GroupState> States;

	public:
		AccessoryGroup();

		void begin();
		//void begin(GroupState *inpFirstState, ...);
		inline void AddState(GroupState *inpState) { this->States.AddItem(inpState); }
		inline void AddState(unsigned long inId, bool inSynchrone = true) { this->States.AddItem(new GroupState(inId, inSynchrone)); }
		void AddStateItem(unsigned long inId, Accessory &inAccessory, ACC_STATE inState, unsigned int inDelay = 0);
		void AddRange(const AccessoryGroup &inGroup);
		GroupState *GetByID(unsigned long inId);
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_TOGGLE, int inData = 0);
		static void EventAll(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_TOGGLE, int inData = 0);

		void StartAction(GroupState *inpState);
		void ResetAction()	{ this->States.ResetCurrent(); }
		bool IsActionPending();

		bool loop();
		bool Toggle(unsigned long inId);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
