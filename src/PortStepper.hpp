//-------------------------------------------------------------------
#ifndef __PortStepper_H__
#define __PortStepper_H__
//-------------------------------------------------------------------

#include "Port.hpp"

#ifndef NO_STEPPER

#include "LocoStepper.h"

/**This class can drive a stepper motor, using the LocoStepper class.




Positions are specified by a signed long integer. At
construction time, the current position of the motor is consider to be 0. Positive
positions are clockwise from the initial position; negative positions are
anticlockwise. The current position can be altered for instance after
initialization positioning.

See LocoStepper class for more information.
*/
class PortStepper: public Port
{
	protected:
		/**Instance of LocoStepper class.*/
		LocoStepper *pMotor;

	public:
		/**Default constructor.*/
		PortStepper();
		
		/** Initialize the instance of two pins stepper motor.
		@param inPin1 Command pin 1 for the stepper.
		@param inPin2 Command pin 2 for the stepper.
		@param inType kind of all the pins. Can be only DIGITAL or DIGITAL_INVERTED. Default is DIGITAL.
		@param inpSteps specific step list. Default is NULL.
		It gives the caller the possibility to fix himself the sequence of pins activation. If nothing
		is specified, a default value will be used:
		\verbatim
		B01, B11, B10, B00.
		\endverbatim
		*/
		void begin(uint8_t inPin1, uint8_t inPin2, PIN_TYPE inType = DIGITAL, uint8_t *inpSteps = NULL);
		/** Initialize the instance for four pins stepper motor.
		@param inPin1 Command pin 1 for the stepper.
		@param inPin2 Command pin 2 for the stepper.
		@param inPin3 Command pin 3 for the stepper.
		@param inPin4 Command pin 4 for the stepper.
		@param inType kind of all the pins. Can be only DIGITAL or DIGITAL_INVERTED. Default is DIGITAL.
		@param inpSteps specific step list. Default is NULL.
		It gives the caller the possibility to fix himself the sequence of pins activation. If nothing 
		is specified, a default value will be used:
		\verbatim
		B1010, B0110, B0101, B1001 for four wires stepper
		\endverbatim
		These steps are those working for a 28BYJ very common and inexpensive small stepper.
		*/
		void begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, PIN_TYPE inType = DIGITAL, uint8_t *inpSteps = NULL);

		/**Move the stepper to the given position using a given duration.
		@param inDuration the movement should take the time of this delay.
		@param inEndPosition final position to reach.
		*/
		void MovePosition(unsigned long inDuration, int inEndPosition);
		/**Move the servo of the given value from the current position using a given duration.
		@param inDuration the movement should take the time of this delay.
		@param inIncrementalPosition position to move.
		*/
		void MoveRelativePosition(unsigned long inDuration, int inIncrementalPosition);

		/** Poll the motor and step it if a step is due. You must call this as
		frequently as possible, but at least once per minimum step interval,
		preferably in your main loop.
		@return true if the motor is still going to the target position.
		*/
		inline bool run() { return this->pMotor->run(); }
		/**Gets the current position.
		@return current position.
		*/
		inline long currentPosition() { return this->pMotor->currentPosition(); }
		/**Gets the target position.
		@return target position. This is equal to current position if no movement is going...
		*/
		inline long targetPosition() { return this->pMotor->targetPosition(); }
		/**Gets the current position.
		@return current position.
		@remark This function is the same than currentPosition() and is only defined to match
		Accessory class functions.
		*/
		inline int GetPosition() { return (int) this->currentPosition(); }

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/**Internal data for print function. */
		uint8_t printPin1;
		/**Internal data for print function. */
		uint8_t printPin2;
		/**Internal data for print function. */
		uint8_t printPin3;
		/**Internal data for print function. */
		uint8_t printPin4;
		/**Internal data for print function. */
		PIN_TYPE printType;
		/**Internal data for print function. */
		uint8_t *printpSteps;
		/** Print this port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printPort();
#endif
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
