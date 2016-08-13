//-------------------------------------------------------------------
#ifndef __accessoryMotorTwoWays_H__
#define __accessoryMotorTwoWays_H__
//-------------------------------------------------------------------

#include "AccessoryMotor.hpp"

//-------------------------------------------------------------------
#ifndef NO_MOTOR
#ifndef NO_MOTORTWOWAYS

class AccessoryMotorTwoWays : public AccessoryMotor
{
	public:
		AccessoryMotorTwoWays(unsigned long inId, unsigned long inDurationMilli = 0);
		AccessoryMotorTwoWays(unsigned long inIdRight, unsigned long inIdLeft, unsigned long inDurationMilli);
};
#endif

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
