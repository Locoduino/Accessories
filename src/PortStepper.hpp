//-------------------------------------------------------------------
#ifndef __PortStepper_H__
#define __PortStepper_H__
//-------------------------------------------------------------------

#include "Port.hpp"

#ifndef NO_STEPPER

#include "LocoStepper.h"

class PortStepper: public Port
{
	protected:
		LocoStepper *pMotor;

	public:
		PortStepper();
		
		void begin(uint8_t inPin1, uint8_t inPin2, PIN_TYPE inType = DIGITAL, uint8_t *inpSteps = NULL);
		void begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, PIN_TYPE inType = DIGITAL, uint8_t *inpSteps = NULL);

		void MovePosition(unsigned long inDuration, int inEndPosition);
		void MoveRelativePosition(unsigned long inDuration, int inEndPosition);

		inline bool run() { return this->pMotor->run(); }
		inline long currentPosition() { return this->pMotor->currentPosition(); }
		inline long targetPosition() { return this->pMotor->targetPosition(); }
		inline int GetPosition() { return (int) this->currentPosition(); }
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
