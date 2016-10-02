//-------------------------------------------------------------------
#ifndef __driverPort2PinsEnable_H__
#define __driverPort2PinsEnable_H__
//-------------------------------------------------------------------

#include "DriverPort2Pins.hpp"

//-------------------------------------------------------------------

#ifndef NO_MOTOR_LIGHT
class DriverPort2PinsEnable : public DriverPort2Pins
{
	protected:
		GPIO_pin_t pinEnable;

	public:
		DriverPort2PinsEnable(uint8_t inId);
		
		void begin(int inPinA, int inPinB, int inPinEnable);
		
		inline int GetPinEnable() const { return GPIO_to_Arduino_pin(this->pinEnable); }
		inline GPIO_pin_t GetPinEnable2() const { return this->pinEnable; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
