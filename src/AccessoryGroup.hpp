//-------------------------------------------------------------------
#ifndef __accessoryGroup_H__
#define __accessoryGroup_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
#include "Chain.hpp"

//-------------------------------------------------------------------

#ifndef NO_GROUP

/**This class describes a group state item : a particular state for a particular accessory.
*/
class GroupStateItem
{
	friend class GroupState;

	private:
		/**Accessory to move.*/
		Accessory *pAccessory;
		/**State to reach when this state is activated.*/
		ACC_STATE State;
		/**The delay gives a duration of the movement, to ensure that this movement is finished before starting the next one.Only used in asynchronous group state.*/
		unsigned int Delay;

	public:
		/**Default constructor.*/
		GroupStateItem() { this->pAccessory = NULL; this->State = STATE_NONE; this->Delay = 0; }
		/**Full constructor.
		@param inpAccessory	Address of the accessory.
		@param inState state to reach.
		@param inDelay The delay gives a duration of the movement, to ensure that this movement is finished before starting the next one.Only used in asynchronous group state. Default is 0.
		*/
		GroupStateItem(Accessory *inpAccessory, ACC_STATE inState, unsigned int inDelay = 0) { this->pAccessory = inpAccessory; this->State = inState; this->Delay = inDelay; }

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print this item on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};

/**This class describes a list of accessories, with an associated state, under one and only one Id.
*/
class GroupState
{
	private:
		/**Id of the state.*/
		unsigned long Id;
		/**Item list of items.*/
		ACCSCHAINEDLIST<GroupStateItem> Items;
		/**If true, the movements of all accessories of the group will be done together.
		If false, each accessory will wait the end of the previous accessory movement (plus its delay) to start moving.
		*/
		bool Synchronous;

	private:
		unsigned long startActionTime;

	public:
		/**Default constructor.*/
		GroupState() { this->Id = UNDEFINED_ID; this->Synchronous = false; }

		/**Full constructor.
		@param inId Id of this state.
		@param inSynchronous If true, the movements of all accessories of the group will be done together.
		If false, each accessory will wait the end of the previous accessory movement (plus its delay) to start moving. Default is true.
		*/
		GroupState(unsigned long inId, bool inSynchronous = true);

		/** Initialize the instance.*/
		void begin();
	
		/**Starts a new action.
		@remark Only for internal usage.
		*/
		void StartAction();

		/**Prepare for a future action
		@remark Only for internal usage.
		*/
		void ResetAction()	{ this->Items.ResetCurrent(); }

		/**Checks if a state item has not been executed.
		@return true if at least one accessory should still move.
		*/
		bool IsActionItemPending()	{ return this->Items.HasCurrent(); }

		/**Gets the id of this state.
		@return  id of this state.
		*/
		unsigned long GetId() const { return this->Id; }

		/**Main loop function.
		@return true if something has been done.
		*/
		void loop();

		/**Add a new state item.
		@param inpAccessory	Address of the accessory.
		@param inState state to reach.
		@param inDelay The delay gives a duration of the movement, to ensure that this movement is finished before starting the next one.Only used in asynchronous group state. Default is 0.
		*/
		void Add(Accessory *inpAccessory, ACC_STATE inState, unsigned int inDelay = 0);

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print this state on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};

/**This class describes a group of accessories.
Each group defines some GroupState to set multiple accessories states with only one Id.
Events handled :

id | type | data | effect
-------- - | ---------------------- - | ---- - | --------------------
Group state id |all|| Execute the group state
*/
class AccessoryGroup
{
	private:
		static AccessoryGroup *pFirstGroup;
		inline void SetNextGroup(AccessoryGroup *inAccessory) { this->pNextGroup = inAccessory; }
		inline AccessoryGroup *GetNextGroup() const { return this->pNextGroup; }
		static void AddGroup(AccessoryGroup *inGroup);
		AccessoryGroup *pNextGroup;
		ACCSCHAINEDLIST<GroupState> States;

	public:
		/*Default constructor.*/
		AccessoryGroup();

		/** Initialize the instance.*/
		void begin();

		/**Add a new state.
		@param inpState Address of the GroupState to add.
		*/
		inline void AddState(GroupState *inpState) { this->States.AddItem(inpState); }

		/**Add a new state.
		@param inId id of this state.
		@param inSynchronous
		*/
		inline void AddState(unsigned long inId, bool inSynchronous = true) { this->States.AddItem(new GroupState(inId, inSynchronous)); }
		/**Add a new state item, into the state of the given id.
		@param inId id of the state to complete.
		@param inAccessory	Accessory.
		@param inState state to reach.
		@param inDelay The delay gives a duration of the movement, to ensure that this movement is finished before starting the next one.Only used in asynchronous group state. Default is 0.
		*/
		void AddStateItem(unsigned long inId, Accessory &inAccessory, ACC_STATE inState, unsigned int inDelay = 0);

		/**Insert a full group into this one.
		@param inGroup group to insert. All states of inGroup will be added to this group.
		*/
		void AddRange(const AccessoryGroup &inGroup);

		/**Gets a state by searching an id.
		@param inId Id to found.
		@return found GroupState address or NULL.
		*/
		GroupState *GetByID(unsigned long inId) const;

		/**Gets the item list from a given GroupState identified by its id.
		@param inId Id to found.
		@return found GroupState item list or NULL.
		*/
		ACCSCHAINEDLISTITEM<GroupState> *GetItemByID(unsigned long inId) const;

		/** Execute a new event. Nothing appends if the Id is not in this group.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		/** Loop through all AccessoryGroup and execute a new event.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		static void EventAll(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		/**Starts a new action.
		@param inpState address of the starting state.
		@remark Only for internal usage.
		*/
		void StartAction(GroupState *inpState);

		/**Prepare for a future action
		@remark Only for internal usage.
		*/
		void ResetAction()	{ this->States.ResetCurrent(); }

		/**Checks if a state item has not been executed.
		@return true if at least one accessory should still move.
		*/
		bool IsActionItemPending();

		/**Main loop function.
		@return true if something has been done.
		*/
		bool loop();

		/**Move to the next GroupState.
		@return returns the new state.
		*/
		bool Toggle(unsigned long inId);

#ifndef NO_EEPROM
		/**Save the data of this group in the EEPROM.
		@remark Only for internal usage.
		*/
		int EEPROMSave(int inPos, bool inSimulate = false);

		/**Reload the data of the group from the EEPROM.
		@remark Only for internal usage.
		*/
		int EEPROMLoad(int inPos);
#endif

		/**Do movements of all the groups.
		@remark Only for internal usage.
		*/
		static bool loops();

		/**Get the total number of groups.
		@remark Only for internal usage.
		*/
		static uint8_t GetCount();

		/**Save the data of all the groups in the EEPROM.
		@remark Only for internal usage.
		*/
		static int EEPROMSaveAll(int inPos, bool inSimulate = false);

		/**Reload the data of all the groups from the EEPROM.
		@remark Only for internal usage.
		*/
		static int EEPROMLoadAll(int inPos);

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print all the groups on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		static void printGroups();
		/** Print this accessory on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
