//-------------------------------------------------------------------
#ifndef __accessoryStepper_H__
#define __accessoryStepper_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#define CALIBRATION	STATE_FIRST

#ifndef NO_STEPPER

/**This class describes a stepper motor powered by a port. See http://www.tigoe.net/pcomp/code/circuits/motors/stepper-motors/ 
to explanations about the stepper motors.
This class can handle two or four coils motors, by using two or four pins and activate them in the right order.
Every stepper motor can differ from a brand/model to another one with the order of activations to apply (the sequence),
so this can be defined manually by the user for the PortStepper of this Accessory.

By construction, this kind of motor can only move with relative values. This class maintains a current position and is able
to move this motor in absolute mode. To continue with construction facts, a stepper motor can have a tolerance for the movements.
*/		
class AccessoryStepper : public Accessory
{
	private:
		unsigned int speed;
		int stepsNumber;
		int reduction;
		double angleByStep;

	public:
		/** Default constructor. */
		AccessoryStepper();

		/**Checks if the accessory can be moved by an absolute value instead of a fix position.
		@return true if the accessory can be moved with move() function using an absolute value.
		*/
		inline bool CanBePositional() const { return true; }

		/** Initialize the instance.
		@param inpPort Port driven this motor.
		@param inStepsNumber Number of steps for a complete rotation of the axis. You can find it in the motor's data-sheet.
		@param inReduction reduction between the motorized axis and the output axis. You can find it in the motor's data-sheet.
		@param inSpeed Speed to control power to the motor. Default is 0.
		@param inMovingPositionsNumber Number of steps for one movement. Default is 1 step.
		*/
		void begin(Port *inpPort, int inStepsNumber, int inReduction, unsigned int inSpeed = 0, int inMovingPositionsNumber = 1);

		/** Execute a new event.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONINDEX, int inData = 0);

//		void SetStartingPosition(int inPosition) { this->currentPos = inPosition; }
		/**Not yet implemented.*/
		void StartCalibration() { this->SetStateRaw(CALIBRATION); }
		/**Not yet implemented.*/
		void EndCalibration() { this->SetStateRaw(STATE_NONE); }

		/**Move to the given absolute value..
		@param[in] inAbsolutePosition new position to reach.
		*/
		void MovePosition(int inAbsolutePosition);

		/**Move of the given value.
		@param[in] inRelativePosition movement to do. Can be positive or negative.
		*/
		void MoveRelativePosition(int inRelativePosition);

		/**Basic moving function.
		@param[in] inId new position id from MovingPosition to reach.
		*/
		void Move(unsigned long inId);

		/**Move to the next MovingPosition.
		@return returns the new state.
		*/
		ACC_STATE MoveToggle();

	private:
		/**Move to the next MovingPosition.
		@return returns the new state.
		*/
		inline ACC_STATE Toggle() { return MoveToggle(); }

		/**Ends the current action of this accessory.
		@return true if the action is not finished.
		@remark Only for internal usage.
		*/
		bool ActionEnded();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
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

