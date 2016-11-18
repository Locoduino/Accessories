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
		
		void begin(Port *inpPort, unsigned long inId, int inSpeed, unsigned long inDurationMilli = 0);
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
#ifndef NO_EEPROM
		int EEPROMLoad(int inPos);
#endif

		void Move(unsigned long inId);
		inline virtual void MoveLeft(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(LEFT, inDuration, inSpeed); }
		inline virtual void MoveRight(unsigned long inDuration = 0, int inSpeed = 0) { this->InternalMove(RIGHT, inDuration, inSpeed); }
		virtual ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0);

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

