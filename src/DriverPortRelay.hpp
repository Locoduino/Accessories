//-------------------------------------------------------------------
#ifndef __driverPortRelay_H__
#define __driverPortRelay_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

#ifndef NO_RELAY
class DriverPortRelay : public DriverPort
{
	protected:
		GPIO_pin_t pin;

	public:
		DriverPortRelay(byte inId);
		
		void begin(int inPin);
		
		inline bool IsActive() const { return this->pin > (GPIO_pin_t) DP_INVALID; }
		inline int GetPin() const { return GPIO_to_Arduino_pin(this->pin); }
		inline GPIO_pin_t GetPin2() const { return this->pin; }
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop(); 
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
