//-------------------------------------------------------------------
#ifndef __driverPort2Pins_H__
#define __driverPort2Pins_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

#ifndef NO_MOTOR_LIGHT
class DriverPort2Pins : public DriverPort
{
	protected:
		GPIO_pin_t pinA, pinB;

	public:
		DriverPort2Pins(uint8_t inId);
		
		void begin(int inPinA, int inPinB);
		
		inline bool IsActive() const { return this->pinA > (GPIO_pin_t)DP_INVALID; }
		inline int GetPinA() const { return GPIO_to_Arduino_pin(this->pinA); }
		inline GPIO_pin_t GetPinA2() const { return this->pinA; }
		inline int GetPinB() const { return GPIO_to_Arduino_pin(this->pinB); }
		inline GPIO_pin_t GetPinB2() const { return this->pinB; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
