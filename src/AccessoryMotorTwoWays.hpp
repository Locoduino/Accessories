//-------------------------------------------------------------------
#ifndef __accessoryMotorTwoWays_H__
#define __accessoryMotorTwoWays_H__
//-------------------------------------------------------------------

#include "AccessoryMotor.hpp"

#ifndef NO_MOTOR
#ifndef NO_MOTORTWOWAYS

/**This class describes a motor with two moving directions : coil motor for turnout...*/
class AccessoryMotorTwoWays : public AccessoryMotor
{
	public:
		/** Default constructor. */
		inline AccessoryMotorTwoWays() {}

		/** Initialize the instance.
		@param inpPort Port driven this motor.
		@param inIdRight Id of this accessory for right moving.
		@param inIdLeft Id of this accessory for left moving.
		@param inSpeed Sets the initial speed for movements.
		@param inDurationMilli Duration in milliseconds of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		*/
		void beginTwoWays(Port *inpPort, unsigned long inIdRight, unsigned long inIdLeft, int inSpeed, unsigned long inDurationMilli = 0);

		/**Change from left to right or right to left.
		@param[in] inDuration Duration of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement. This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		@return returns the new state.
		*/
		ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0);

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print this accessory on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};
#endif

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
