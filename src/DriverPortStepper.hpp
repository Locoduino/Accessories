//-------------------------------------------------------------------
#ifndef __driverPortStepper_H__
#define __driverPortStepper_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"
#include "Stepper.h"

//-------------------------------------------------------------------

#ifndef NO_STEPPER

#include "LocoStepper.h"

class DriverPortStepper: public DriverPort
{
	protected:
		AccelStepper *pMotor;

		uint8_t powerCommandPin[4];
	public:
		DriverPortStepper(uint8_t inId);
		
		void begin(uint8_t inPin1, uint8_t inPin2, uint8_t *inpSteps = NULL);
		void begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, uint8_t *inpSteps = NULL);

		void MovePosition(unsigned long inDuration, int inEndPosition);
		void MoveRelativePosition(unsigned long inDuration, int inEndPosition);

		inline bool run() { return this->pMotor->run(); }
		inline long currentPosition() { return this->pMotor->currentPosition(); }
		inline long targetPosition() { return this->pMotor->targetPosition(); }
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
