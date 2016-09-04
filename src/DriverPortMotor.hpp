//-------------------------------------------------------------------
#ifndef __driverPortMotor_H__
#define __driverPortMotor_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

class DriverPortMotor : public DriverPort
{
	protected:
		GPIO_pin_t pin;

	public:
		DriverPortMotor(uint8_t inId);
		
		void begin(int inPin);
		
		inline bool IsActive() const { return this->pin > (GPIO_pin_t) DP_INVALID; }
		inline int GetPin() const { return GPIO_to_Arduino_pin(this->pin); }
		inline GPIO_pin_t GetPin2() const { return this->pin; }
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop(); 
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
