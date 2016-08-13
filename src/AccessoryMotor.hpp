//-------------------------------------------------------------------
#ifndef __accessoryMotor_H__
#define __accessoryMotor_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
class Driver;

//-------------------------------------------------------------------

#ifndef NO_MOTOR

// This class describes a motor powered by a driver.

#define LEFT	STATE_FIRST
#define RIGHT	STATE_SECOND
#define STOP	STATE_NONE

enum DIRECTION { ONLYLEFT, ONLYRIGHT, BOTHDIRS };

class AccessoryMotor : public Accessory
{
	public:
		AccessoryMotor();
		bool ActionEnded();
		
		inline bool IsLeft() const { return this->IsFirst(); }
		inline bool IsRight() const { return this->IsSecond(); }
		inline bool IsStopped() const { return this->IsNone(); }
		
		void begin(DriverPort *inpPort, unsigned long inId, int inSpeed, unsigned long inDurationMilli = 0);
		void Event(ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_TOGGLE, int inData = 0);

	protected:
		void SetState(ACC_STATE instate);
		void Move(unsigned long inId);
		inline virtual void MoveLeft(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(LEFT, inDuration, inSpeed); }
		inline virtual void MoveRight(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(RIGHT, inDuration, inSpeed); }
		virtual ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0);
		inline ACC_STATE Toggle() { return MoveToggle(); }

	private:
		ACC_STATE InternalMove(ACC_STATE inStateToReach, unsigned long inDuration = 0, int inSpeed = 0);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------

