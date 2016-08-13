//-------------------------------------------------------------------
#ifndef __accessoryMotorOneWay_H__
#define __accessoryMotorOneWay_H__
//-------------------------------------------------------------------

#include "AccessoryMotor.hpp"

//-------------------------------------------------------------------
#ifndef NO_MOTOR
#ifndef NO_MOTORONEWAY

class AccessoryMotorOneWay : public AccessoryMotor
{
	public:
		inline AccessoryMotorOneWay() {}
		
	protected:
		inline void MoveRight(unsigned long inDuration = 0, int inSpeed = 0) { MoveLeft(inDuration, inSpeed); }
		inline ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0) { MoveLeft(inDuration, inSpeed); return LEFT; }
};
#endif 

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
