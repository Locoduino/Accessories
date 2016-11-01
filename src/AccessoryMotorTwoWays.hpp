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
		inline AccessoryMotorTwoWays() {}
		void beginTwoWays(Port *inpPort, unsigned long inIdRight, unsigned long inIdLeft, int inSpeed, unsigned long inDurationMilli = 0);
};
#endif

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
