//-------------------------------------------------------------------
#ifndef __accessoryStepper_H__
#define __accessoryStepper_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#define CALIBRATION	STATE_FIRST

#ifndef NO_STEPPER

// This class describes a stepper motor powered by a driver.

class AccessoryStepper : public Accessory
{
	protected:
		unsigned int speed;
		int stepsNumber;
		int reduction;
		double angleByStep;

	public:
		AccessoryStepper();

		inline bool CanBePositionnal() const { return true; }

		void begin(Port *inpPort, int stepsNumber, int reduction, unsigned int inSpeed = 0, int inMovingPositionsNumber = 1);
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONINDEX, int inData = 0);
//		void SetStartingPosition(int inPosition) { this->currentPos = inPosition; }
		void StartCalibration() { this->SetStateRaw(CALIBRATION); }
		void EndCalibration() { this->SetStateRaw(STATE_NONE); }

	private:
		void MovePosition(int inAbsolutePosition);
		void MoveRelativePosition(int inRelativePosition);
		void Move(unsigned long inId);
		ACC_STATE MoveToggle();
		inline ACC_STATE Toggle() { return MoveToggle(); }
		bool ActionEnded();
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------

