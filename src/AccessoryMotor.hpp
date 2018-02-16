//-------------------------------------------------------------------
#ifndef __accessoryMotor_H__
#define __accessoryMotor_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
class Driver;

//-------------------------------------------------------------------

#ifndef NO_MOTOR

// This class describes a motor powered by a driver.

/** Re-define STATE_FIRST as LEFT.*/
#define LEFT	STATE_FIRST
/** Re-define STATE_FIRST as RIGHT.*/
#define RIGHT	STATE_SECOND
/** Re-define STATE_FIRST as STOP.*/
#define STOP	STATE_NONE

/**Angular direction form a rotating motor.*/
enum DIRECTION
{ 
	ONLYLEFT,	/**<Rotate only in left direction.*/
	ONLYRIGHT, 	/**<Rotate only in right direction.*/
	BOTHDIRS	/**<Can rotate in both directions.*/
};

/**This class describes a classic rotative or linear motor.
This is an abstract class, which cannot be instantiated.
*/
class AccessoryMotor : public Accessory
{
	public:
		/** Default constructor. */
		AccessoryMotor();

		/**Ends the current action of this accessory.
		@return true if the action is not finished.
		@remark Only for internal usage.
		*/
		bool ActionEnded();
		
		/**Checks if current state is LEFT, or STATE_FIRST in basic accessory word.
		@return true if the current state is LEFT.
		*/
		inline bool IsLeft() const { return this->IsFirst(); }

		/**Checks if current state is RIGHT, or STATE_SECOND in basic accessory word.
		@return true if the current state is RIGHT.
		*/
		inline bool IsRight() const { return this->IsSecond(); }

		/**Checks if current state is STOP, or STATE_NONE in basic accessory word.
		@return true if the current state is STOP.
		*/
		inline bool IsStopped() const { return this->IsNone(); }

		/** Initialize the instance.
		@param inpPort Port driven this motor.
		@param inId Id of this accessory.
		@param inSpeed Sets the initial speed for movements.
		@param inDurationMilli Duration in milliseconds of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		*/
		void begin(Port *inpPort, unsigned long inId, int inSpeed, unsigned long inDurationMilli = 0);

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

		/**Basic moving function.
		@param[in] inId new position id from MovingPosition to reach.
		*/
		void Move(unsigned long inId);

		/**Move to left position.
		@param[in] inDuration Duration of the movement.	If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement. This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		*/
		inline virtual void MoveLeft(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(LEFT, inDuration, inSpeed); }

		/**Move to right position.
		@param[in] inDuration Duration of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement.  This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		*/
		inline virtual void MoveRight(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(RIGHT, inDuration, inSpeed); }

		/**Change from left to right or right to left.
		@param[in] inDuration Duration of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement. This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		@return returns the new state.
		*/
		virtual ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0) = 0;

		/**Stop the movement.*/
		virtual void MoveStop();

		/**Change the state of the motor, to reflect a manual or external movement.
		@param[in] inNewState new state.
		*/
		void ExternalMove(ACC_STATE inNewState) { this->SetStateRaw(inNewState); }

	protected:
		void SetState(ACC_STATE instate);
		inline ACC_STATE Toggle() { return MoveToggle(); }
	private:
		ACC_STATE InternalMove(ACC_STATE inStateToReach, unsigned long inDuration = 0, int inSpeed = 0);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------

