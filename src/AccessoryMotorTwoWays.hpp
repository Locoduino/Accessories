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
		ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0);
#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		void printAccessory();
#endif
};
#endif

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
