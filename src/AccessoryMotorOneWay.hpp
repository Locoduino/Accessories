//-------------------------------------------------------------------
#ifndef __accessoryMotorOneWay_H__
#define __accessoryMotorOneWay_H__
//-------------------------------------------------------------------

#include "AccessoryMotor.hpp"

#ifndef NO_MOTOR
#ifndef NO_MOTORONEWAY

/**This class describes a motor with only one moving direction : uncoupling motor, rotating motor for windmill sails...*/
class AccessoryMotorOneWay : public AccessoryMotor
{
	public:
		/** Default constructor. */
		inline AccessoryMotorOneWay() {}
		
	protected:
		/**Move the motor... There is only one direction, so MoveLeft() will do the same movement than MoveRight() !
		@param[in] inDuration Duration of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement. This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		*/
		inline void MoveRight(unsigned long inDuration = 0, int inSpeed = 0) { MoveLeft(inDuration, inSpeed); }

		/**Change the direction. Huh ? There is only one direction ? OK, change the direction to ... the same !
		@param[in] inDuration Duration of the movement. If 0, the movement will not stop until a MoveStop(). Default is 0.
		@param[in] inSpeed speed for this movement. This speed becomes the default speed for next movements. If 0, the speed is not updated. Default is 0.
		@return returns the new state.
		*/
		inline ACC_STATE MoveToggle(unsigned long inDuration = 0, int inSpeed = 0) { MoveLeft(inDuration, inSpeed); return LEFT; }

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
