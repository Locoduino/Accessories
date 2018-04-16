//-------------------------------------------------------------------
#ifndef __accessory_H__
#define __accessory_H__
//-------------------------------------------------------------------

#include "DIO2.h"

//-------------------------------------------------------------------

#ifndef UNDEFINED_ID
/** Undefined id ! Used by all loop() functions if no event has been detected.*/
#define UNDEFINED_ID	((unsigned long)-1)
#endif

#ifndef UNDEFINED_POS
/** Undefined position ! Used by GetMovingPosition() function when a position is not relevant (light...).*/
#define UNDEFINED_POS	(-32767)
#endif

/**	Describes the associated data with some particular cases of event type ACCESSORIES_EVENT_MOVE.
These values will be interpreted by the receiving object. Some values could not have meaning in the context...
This enum is almost identical to the enum COMMANDERS_MOVE_TYPE in Commanders library.
*/
enum ACCESSORIES_MOVE_TYPE
{
	ACCESSORIES_MOVE_MORE = +1,		/**< Add an incremental value, like an encoder can do.*/
	ACCESSORIES_MOVE_LESS = -1,		/**< Subtract an incremental value, like an encoder can do.*/
	ACCESSORIES_MOVE_STOP = 0,		/**< Stop the current movement.*/
	ACCESSORIES_MOVE_LEFT = -2,		/**< Move to left side.*/
	ACCESSORIES_MOVE_RIGHT = -3,	/**< Move to right side.*/
	ACCESSORIES_MOVE_CENTER = -4,	/**< Move to center.*/
	ACCESSORIES_MOVE_TOP = -5,		/**< Move to top.*/
	ACCESSORIES_MOVE_BOTTOM = -6,	/**< Move to bottom.*/
	ACCESSORIES_MOVE_STRAIGHT = -7,	/**< Set the turnout to straight position.*/
	ACCESSORIES_MOVE_DIVERGE = -8,	/**< Set the turnout to deviated position.*/
	ACCESSORIES_MOVE_ON = -9,		/**< Set accessory on.*/
	ACCESSORIES_MOVE_OFF = -10		/**< Set accessory off.*/
};

/**	Describes the main events received or thrown by accessories.
This enum is almost identical to the enum COMMANDERS_EVENT_TYPE in Commanders library.
Types with value greater than 20 are not common with Commanders.
*/
enum ACCESSORIES_EVENT_TYPE
{
	ACCESSORIES_EVENT_NONE = 0,				/**< Should never appear.*/
	ACCESSORIES_EVENT_TOGGLE = 1,			/**< If a push button or similar is pressed, invert the state/position.*/
	ACCESSORIES_EVENT_MOVE = 2,				/**< If a push 'left' button or similar is pressed, data is a COMMANDERS_MOVE_TYPE.*/
	ACCESSORIES_EVENT_MOVEPOSITION = 3,		/**< Move to a given position value given by data.*/
	ACCESSORIES_EVENT_MOVEPOSITIONINDEX = 4,/**< Move to an indexed position from pMovingPosition given by data.*/
	ACCESSORIES_EVENT_MOVEPOSITIONID = 5,	/**< Move to an identified position from pMovingPosition, given by the event id.*/
	ACCESSORIES_EVENT_CONFIG = 6,			/**< Data is the configuration address and value.*/

	ACCESSORIES_EVENT_DIRECTION = 20,		/**< Move in positive or negative direction.*/
	ACCESSORIES_EVENT_SETSTARTPOSITION = 21,/**< Change the starting position of the motor.*/
	ACCESSORIES_EVENT_SETSPEED = 22,		/**< Change the speed for a motor or the intensity for a light.*/
	ACCESSORIES_EVENT_EXTERNALMOVE = 23,	/**< Manually change the state for a motor without using the motor ! Does not apply to a light. New ACC_STATE must be in data.*/
	ACCESSORIES_EVENT_SETDURATION = 24,		/**< Change the duration of a movement for a motor or the blinking delay for a light.*/
};

#define ACCESSORIESSCONFIG(address, value)	( ((int)address<<8) | value )
#define ACCESSORIESCONFIGADDRESS(data)		highByte((int)data)
#define ACCESSORIESCONFIGVALUE(data)		lowByte((int)data)

/**Defines three states for the accessory. States can be redefined locally by macro to match with real goal...
STATE_SECOND is STATE_RIGHT for turnout or STATE_LIGHTBLINK for a light !*/
enum ACC_STATE
{ 
	STATE_NONE = 0,		/**< No state defined.*/ 
	STATE_FIRST = 1,	/**< First defined state.*/
	STATE_SECOND = 2	/**< Second defined state.*/
};

/**Defines one position by an id.*/
struct MovingPosition
{
	unsigned long Id;	/**< Id of this position.*/
	int Position;		/**< Actual position.*/
};

/**
This is the base class for all the accessories.

This class is a pure virtual class, so it cant be instantiated !

An accessory is a class driving a physical motor or light, receiving events to change its state.
With each accessory is defined a port, giving power to the accessory.
There is no accessory without port.

Each accessory have one or more MovingPosition, with each one defined by an Id.
Each accessory have also a current state, a current speed and can have a current position.
All these states are saved in EEPROM at each state change, with a protection to avoid more than one saving by second.

The EEPROM area starting from EEPROMStart will be filled on EEPROMsize with the current version number (a byte),
and the total number of accessories and groups. The fourth byte will be the checksum of the three
starting bytes...
If any of these four bytes are different from the actual values, the EEPROM will be considered as free.
Two bytes are then added to store the size of one complete record, in order to be able to configure the CircularBuffer.

Following these bytes, there is the CircularBuffer of accessories.
For each one the current state, the current position and the current speed will be saved.
After that, each group save its current item id.

\verbatim
+-+
Version					|V|
Accessory number		|A|
Group number			|G|
header checksum			|C|
size byte 1				|s1]
size byte 2				|s2]
+-+
Circular buffer			| |
+-+
End of file.
\endverbatim

And the Circular buffer is :

\verbatim
+-+
Acc 1 : State			|S|
Acc 1 : Position		|P|
Acc 1 : Speed			|s|
+-+
Acc 2 : State			|S|
Acc 2 : Position		|P|
Acc 2 : Speed			|s|
+-+
Acc 3 : State			|S|
Acc 3 : Position		|P|
Acc 3 : Speed			|s|
...						+-+
Group 1 : current ID	|I|
Group 3 : current ID	|I|
Group 3 : current ID	|I|
+-+
End of record.
\endverbatim
*/
class Accessory
{
private:
	static Accessory *pFirstAccessory;
	Accessory *pNextAccessory;
	
	uint8_t movingPositionsSize;
	uint8_t movingPositionsAddCounter;

	// RAM storage for moving positions
	MovingPosition *pMovingPositions;

	// Program memory storage for moving positions
	const MovingPosition *pMovingPositions_P;

	unsigned long duration;
	unsigned long startingMillis;
	uint8_t lastMovingPosition;
	unsigned int debounceDelay;
	unsigned long lastMoveTime;

	ACC_STATE state;

	static void Add(Accessory *inpAccessory);
	static Accessory *GetById(unsigned long inId);
	static bool CanMove(unsigned long inId);
	bool IsMovementPending();
	static bool Toggle(unsigned long inId);
	static bool MovePosition(unsigned long inId);

	/**Gets the position at the given index.
	@param inIndex number of the wanted index.
	@return Found MovingPosition or NULL if not found.
	*/
	MovingPosition *GetMovingPositionByIndex(uint8_t inIndex, MovingPosition *inPosition) const;
	/**Gets the position with the given id.
	@param inId Id to find.
	@return Found MovingPosition or NULL if not found.
	*/
	MovingPosition *GetMovingPositionById(unsigned long inId, MovingPosition *inPosition) const;

	bool IsPPointer() const { return this->pMovingPositions_P != NULL; }

protected:
	/**Port to drive the accessory.
	An accessory should always have a port, even if this is the Arduino itself...*/
	Port *pPort;

	/**Previous state of the accessory.*/
	ACC_STATE prevState;

public:
	/** Default constructor. */
	Accessory();

	/** Initialize the instance.
	@param inStartingState		Starting state of the accessory. This is NOT an order to set the accessory in this state, only a way to
	take account of the current physical state of it !
	*/
	inline void begin(ACC_STATE inStartingState = STATE_NONE) { this->state = inStartingState; }

	/**Gets the associated port.
	@return port address or NULL.
	*/
	inline Port *GetPort() const { return this->pPort; }

	/**Execute the event.*/
	static void ExecuteEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
	/**Gets the number of accessories.
	@return total number of accessories.
	*/
	static uint8_t GetCount();

	/**Checks if there is at least one accessory which needs more loop() calls.
	@return true if one accessory needs more 'loop()'.
	*/
	static bool IsActionPending();

#ifdef ACCESSORIES_DEBUG_MODE
	/** checks if the port has been defined.
	@remark Only available if ACCESSORIES_DEBUG_MODE is defined.
	*/
	virtual void CheckPort() const;
#endif

	/**Gets the current state of the accessory.
	@return current state.
	*/
	inline ACC_STATE GetState() const { return this->state; }

	/**Gets the previous state of the accessory, when relevant.
	@return Previous state.
	@remark Only for internal usage.
	*/
	inline ACC_STATE GetPreviousState() const { return this->prevState; }

	/**Checks if current state is STATE_NONE.
	@return true if the current state is STATE_NONE.
	*/
	inline bool IsNone() const { return this->state == STATE_NONE; }

	/**Checks if current state is STATE_FIRST.
	@return true if the current state is STATE_FIRST.
	*/
	inline bool IsFirst() const { return this->state == STATE_FIRST; }

	/**Checks if current state is STATE_SECOND.
	@return true if the current state is STATE_SECOND.
	*/
	inline bool IsSecond() const { return this->state == STATE_SECOND; }

	/**Gets the debounce delay of the accessory, when relevant.
	@return Previous debounce delay in milliseconds.
	*/
	inline unsigned int GetDebounceDelay() const { return this->debounceDelay; }

	/**Gets the time of the last change of this accessory.
	A move can be done only if the elapsed time since last move is bigger than 'debounce' delay.
	@return last time of move in milliseconds.
	*/
	inline unsigned long GetLastMoveTime() const { return this->lastMoveTime; }

	/**Gets the duration. Some accessories needs a duration: blinking light, step for servo motor...
	@return duration for this accessory. 0 if no duration.
	*/
	inline unsigned long GetDuration() const { return this->duration; }

	/**Main loop function.
	@return true if something has been done.
	*/
	inline bool loop() { return this->ActionEnded(); }

	/** Execute a new event.
	@param inId Id of an accessory or an accessory item.
	@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
	@param inData Associated data to the event type. Default is 0.
	*/
	inline virtual void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0) {}

public:	//but should be protected !
#ifndef NO_EEPROM
	/**Save all the data in the EEPROM.
	@remark Only for internal usage.
	*/
	virtual int EEPROMSave(int inPos, bool inSimulate = false);

	/**Reload all data from the EEPROM.
	@remark Only for internal usage.
	*/
	virtual int EEPROMLoad(int inPos);

#endif
	/**Add one moving position to the accessory.
	@param[in] inId new Id
	@param[in] inPosition	Position for this id.
	@return index of the new position in the positions list.
	@remark the total number of MovingPosition must be set before with AdjustMovingPositionsSize().
	*/
	uint8_t AddMovingPosition(unsigned long inId, int inPosition);

	/**Add a list of moving positions declared with PROGMEM to the accessory.
	@param[in] inMovingPositions_P list of MovingPosition defined with PROGMEM attribute.
	@return number of positions in the list.
	*/
	uint8_t AddMovingPositions(const MovingPosition *inMovingPositions_P);

	/**Sets the number of MovingPosition. If some MovingPosition was previously existing, they will be preserved.
	@param inNewSize new size.
	*/
	void AdjustMovingPositionsSize(uint8_t inNewSize);

	/**Checks if at least one MovingPosition has been defined.
	@return true if there is at least one MovingPosition defined.
	*/
	inline bool IsEmpty() const { return this->pMovingPositions == NULL && this->pMovingPositions_P == NULL; }

	/**Find the index of a MovingPosition from its Id.
	@param inId	id of the MovingPosition to find.
	@return index of the found MovingPosition or 255 if not found.
	*/
	uint8_t IndexOfMovingPositionById(unsigned long inId) const;

	/**Gets the position associated with the given Id.
	@param inId	id of the MovingPosition to find.
	@return position or UNDEFINED_POS if not found..
	*/
	inline int GetMovingPositionValueById(unsigned long inId) const { MovingPosition mp;  return this->GetMovingPositionById(inId, &mp)->Position; }

	/**Gets the position at the given index of the list.
	@param inIndex	index of the MovingPosition in the list.
	@return position.
	*/
	inline int GetMovingPositionValueByIndex(uint8_t inIndex) const { MovingPosition mp;  return this->GetMovingPositionByIndex(inIndex, &mp)->Position; }

	/**Gets the id of the MovingPosition at the given index of the list.
	@param inIndex	index of the MovingPosition in the list.
	@return Id of this position.
	*/
	inline unsigned long GetMovingPositionIdByIndex(uint8_t inIndex) const { MovingPosition mp;  return this->GetMovingPositionByIndex(inIndex, &mp)->Id; }

	/**Gets the last position reached by this accessory.
	@return last position reached by the accessory, or 255 if it never moved.
	*/
	inline const uint8_t GetLastMovingPosition() const { return this->lastMovingPosition; }

	/** Gets the number of MovingPosition.
	@return Number of MovingPosition set by AdjustMovingPositionsSize().
	*/
	inline const uint8_t GetMovingPositionSize() const { return this->movingPositionsSize; }

	/**Sets the last position reached by this accessory.
	@param[in] inLastPositionIndex last position reached by the accessory.
	@remark Only for internal usage.
	*/
	void SetLastMovingPosition(uint8_t inLastPositionIndex);

	/**Sets the debounce delay. This delay is used to be sure that the current move is finished. A new move will only operate if this delay is expired.
	@param[in] inDebounceDelay delay needed to finish an action (movement...) of the accessory.
	*/
	inline void SetDebounceDelay(unsigned int inDebounceDelay) { this->debounceDelay = inDebounceDelay; }

	/**Sets the last move time from the current time.
	@remark Only for internal usage.
	*/
	inline void SetLastMoveTime() { this->lastMoveTime = millis(); }

	/**Sets duration. This duration is used by lights for blinking, or by servos for the interval between elementary movements.
	@param[in] inDuration duration of the accessory.
	*/
	inline void SetDuration(unsigned long inDuration) { this->duration = inDuration; }

	/**Sets	the next accessory in the list.
	@param[in] inAccessory address of the next accessory in the list.
	@remark Only for internal usage.
	*/
	inline void SetNextAccessory(Accessory *inAccessory) { this->pNextAccessory = inAccessory; }

	/**Sets	the first accessory address in the list.
	@return address of the first accessory in the list.
	*/
	inline static Accessory *GetFirstAccessory() { return pFirstAccessory; }

	/**Gets	the next accessory in the list.
	@return address of the next accessory in the list.
	*/
	inline Accessory *GetNextAccessory() const { return this->pNextAccessory; }

	/**Gets	the time of the starting of the current movement.
	@return starting time of the current movement.
	@remark Only for internal usage.
	*/
	inline unsigned long GetActionStartingMillis() const { return this->startingMillis; }

	/**Starts a new action.
	@remark Only for internal usage.
	*/
	virtual void StartAction();

	/**Starts a new action and sets the current state of this accessory.
	@param[in] inState new current state of this accessory.
	@remark Only for internal usage.
	*/
	virtual void StartAction(ACC_STATE inState);

#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	virtual void ResetAction();
#else
	/**Prepare for a future action
	@remark Only for internal usage.
	*/
	inline virtual void ResetAction() { this->startingMillis = 0; }
#endif

	/**Ends the current action of this accessory.
	@return true if the action is not finished.
	@remark Only for internal usage.
	*/
	virtual bool ActionEnded();

	/**Checks if an action is still pending.
	@return true if an action is not ended.
	@remark Only for internal usage.
	*/
	inline bool IsActionDelayPending() const { return this->startingMillis > 0; }

	/**Checks if an action is still pending on a group.
	@return true if an action is not ended or if all the items of the parent group have not been actioned.
	@remark Only for internal usage.
	*/
	inline virtual bool IsGroupActionPending() const { return this->IsActionDelayPending(); }

	/**Basic moving function.
	@param[in] inMovingPositionId new position id from MovingPosition to reach.
	*/
	virtual void Move(unsigned long inMovingPositionId) = 0;

	/**Basic function to toggle the accessory. A turnout will change its position, a light will change its state to on or off depending of its current state...
	@return new state of the accessory.
	*/
	virtual ACC_STATE Toggle() = 0;

	/**Checks if the accessory can be moved by an absolute value instead of a fix position.
	@return true if the accessory can be moved with move() function using an absolute value.
	*/
	inline virtual bool CanBePositional() const { return false; }

	/**Basic moving function for a positional motor.
	@param[in] inPosition new position to reach.
	@remark this function can be used only if CanBePositional() returns true. 
	*/
	inline virtual void MovePosition(int inPosition) {}

	/**Change the state of the accessory. The accessory will physically move to reach the new state.
	@param inNewState new state to reach.
	*/
	inline virtual void SetState(ACC_STATE inNewState) { this->SetStateRaw(inNewState); }

	/**Change the state of the accessory. The accessory will NOT move to reach the new state.
	@param inNewState new state to reach.
	@remark Only for internal usage.
	*/
	void SetStateRaw(ACC_STATE inNewState);

	/**Sets the starting time of a new action.
	@remark Only for internal usage.
	*/
	inline void SetStartingMillis() { this->startingMillis = millis(); }

	/**Resets the starting time of the current action.
	@remark Only for internal usage.
	*/
	inline void ResetStartingMillis() { this->startingMillis = 0; }

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	/** Print one accessory on console.
	@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
	*/
	virtual void printAccessory() {
		if (this->GetPort() != NULL)
			this->GetPort()->printPort();

		this->printMovingPositions();
	}

	/** Print the MovingPosition list on console.
	@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
	*/
	void printMovingPositions();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
