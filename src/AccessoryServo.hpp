//-------------------------------------------------------------------
#ifndef __accessoryServo_H__
#define __accessoryServo_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#ifndef NO_SERVO

/** Re-define STATE_FIRST as MINIMUM.*/
#define MINIMUM		STATE_FIRST
/** Re-define STATE_SECOND as MAXIMUM.*/
#define MAXIMUM		STATE_SECOND

/** As servo can move with four ways. This value is internally computed from the duration of each movement step given in the begin().*/
enum MovementSpeed
{
	/** In fast movement, the full movement is done as fast as possible, in one servo function call. There is no delay, no wait.
	During the movement, other accessories cannot answer to user action.*/
	ServoFast,

	/** The movement is done degree by degree, with a small delay (less than 5ms) at the end of each movement.
	During the movement, other accessories cannot answer to user action.*/
	ServoAlone,

	/** The movement is done degree by degree, with a medium delay (6ms to 19ms) at the end of each movement.
	During the movement, other accessories cannot answer to user action, but these actions are stored in a stack
	to be executed when the movement will end.*/
	ServoActionStack,

	/** The movement is done degree by degree, with a long delay (greater than or equal to 20ms) at the end of each movement.
	During the movement, other accessories can handle user actions and move if necessary. This is also the only way to
	move more than one servo at the same time.*/
	ServoSlow
};

enum POWER_STATE
{
	PowerNoAction,
	PowerBeforeRunning,
	PowerRunning,
	PowerAfterRunning
};

/** This class describes a servo powered by a port. A servo has a minimum, a maximum and a current position.

A movement can be split into small steps with a duration for each one. 

Use AddMinMaxMovingPositions() to define MovingPosition for minimum and maximum positions.
Use AddMovingPosition() to define intermediate positions.

To avoid erratic movements when the servo don't move, its power can be stopped by a pin connected to a relay or a transistor.
In this case, the movement is:

\verbatim
MovePosition()

Activate pin, power delay, full movement, power delay, Dis-activate pin.
\endverbatim  

Events handled:

id       |         type          | data| effect
---------|-----------------------|-----|--------------------
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STRAIGHT | Moves the servo to minimum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_TOP | Moves the servo to minimum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LEFT | Moves the servo to minimum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_DIVERGE | Moves the servo to maximum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_BOTTOM | Moves the servo to maximum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_RIGHT | Moves the servo to maximum position...
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_MORE | Moves the servo of 1 degree.
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LESS | Moves the servo of -1 degree.
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STOP | Stops the servo.
servo id | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_OFF | Stops the servo.
servo id | ACCESSORIES_EVENT_MOVEPOSITION | Value in degrees | Moves the servo to the given position.
servo position id | ACCESSORIES_EVENT_TOGGLE ||Use the id to reach this moving position
servo position id | ACCESSORIES_EVENT_MOVEPOSITIONID||Use the id to reach this moving position
servo position id | ACCESSORIES_EVENT_MOVEPOSITIONINDEX|Index|Use the index to reach this moving position
servo id | ACCESSORIES_EVENT_SETSPEED | New speed | Change the current speed.
servo id | ACCESSORIES_EVENT_SETDURATION | New duration | Change the current duration. 0 for continual movement.
servo id | ACCESSORIES_EVENT_EXTERNALMOVE | New state | Change the current state of the object, without doing any movement on the real motor.
*/
class AccessoryServo : public Accessory
{
	private:
		int currentPosition;
		int minimumPosition;
		int maximumPosition;
		int targetSpeed;
		int targetPosition;
		GPIO_pin_t powerCommandPin;
		unsigned long powerDelay;
		POWER_STATE powerState;

	public:
		/** Default constructor. */
		AccessoryServo();

		/**Gets the minimum position of this servo.
		@return minimum position in degrees.
		*/
		inline int GetMinimumPosition() const { return this->minimumPosition; }

		/**Gets the maximum position of this servo.
		@return maximum position in degrees.
		*/
		inline int GetMaximumPosition() const { return this->maximumPosition; }

		/**Gets the current position of this servo.
		@return current position in degrees.
		*/
		inline int GetCurrentPosition() const { return this->currentPosition; }

		/**Sets the minimum and maximum positions.
		@param inMinimum minimum position in degrees.
		@param inMaximum maximum position in degrees.
		*/
		inline void SetMinMax(int inMinimum, int inMaximum) { this->minimumPosition = inMinimum; this->maximumPosition = inMaximum; }

		/**Defines two MovingPosition for minimum and maximum positions.
		@param inIdMin id for minimum position.
		@param inIdMax id for maximum position.
		*/
		inline void AddMinMaxMovingPositions(unsigned long inIdMin, unsigned long inIdMax) { this->AddMovingPosition(inIdMin, this->minimumPosition); this->AddMovingPosition(inIdMax, this->maximumPosition); }
		
		/**Checks if the minimum position is reached.
		@return true it the current position if lower or equals to minimal position..
		*/
		inline bool IsMinimumOrLowerPosition() const { return this->currentPosition <= this->minimumPosition; }

		/**Checks if the maximum position is reached.
		@return true it the current position if greater or equals to maximal position..
		*/
		inline bool IsMaximumOrGreaterPosition() const { return this->currentPosition >= this->maximumPosition; }

		/**Checks if the current position is in the limits.
		@return true it the current position if greater to minimal position and lower then maximum position.
		*/
		inline bool IsMiscPosition() const { return this->currentPosition < this->maximumPosition && this->currentPosition > this->minimumPosition; }

		/**Gets the movement speed computed internally.
		@return MovementSpeed.*/
		MovementSpeed GetMovementSpeed() const;

		/**Checks if the accessory can be moved by an absolute value instead of a fix position.
		@return true if the accessory can be moved with move() function using an absolute value.
		*/
		inline bool CanBePositional() const { return true; }

		/** Initialize the instance.
		@param inpPort Port driven this motor.
		@param inDurationMilli Duration in milliseconds of a step for the movement. If 0, the movement will go as fast as possible. Default is 0.
		@param inMinimumPosition Minimum position in degrees. Default is 0.
		@param inMaximumPosition Maximum position in degrees. Default is 180.
		@param inMovingPositionsNumber Size of a step for movement. Default is 1 degree.
		*/
		void begin(Port *inpPort, unsigned long inDurationMilli = 0, int inMinimumPosition = 0, int inMaximumPosition = 180, int inMovingPositionsNumber = 1);

		/**Sets the pin to control the servo power.
		@param inPin pin to set to HIGH to activate servo power.
		@param inDelay delay to wait after the activation before starting the movement, and after the end of the movement before inactivate the pin.
		*/
		void SetPowerCommand(int inPin, unsigned long inDelay = 100);

		/** Execute a new event.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
#ifndef NO_EEPROM
		/**Reload all data from the EEPROM.
		@remark Only for internal usage.
		*/
		int EEPROMLoad(int inPos);
#endif

		/**Move to the given position.
		@param[in] inPosition new position to reach in degrees.
		*/
		void MovePosition(int inPosition);

		/**Basic moving function.
		@param[in] inId new position id from MovingPosition to reach.
		*/
		void Move(unsigned long inId);

		/**Move to minimum position.*/
		void MoveMinimum();

		/**Move to maximum position.*/
		void MoveMaximum();

		/**Change from minimum to maximum or maximum to minimum.
		@return returns the new state.
		*/
		ACC_STATE MoveToggle();

		/**Change the state of the servo, to reflect a manual or external movement.
		@param[in] inNewState new state.
		*/
		void ExternalMove(ACC_STATE inNewState) { this->SetStateRaw(inNewState); }

	private:
		void SetState(ACC_STATE instate);
		inline ACC_STATE Toggle() { return MoveToggle(); }
		bool ActionEnded();
		void InternalMovePosition(int inPosition);
		void PowerOn();
		void PowerOff();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print one accessory on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};
#endif
#endif

