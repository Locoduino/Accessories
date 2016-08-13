//-------------------------------------------------------------------
#ifndef __driverPortL298n_H__
#define __driverPortL298n_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

#ifndef NO_L298N
class DriverPortL298n : public DriverPort
{
	protected:
		GPIO_pin_t pinA, pinB;

	public:
		DriverPortL298n(byte inId);
		
		void begin(int inPinA, int inPinB);
		
		uint8_t SetSpeed(uint8_t inSpeed);
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
