//-------------------------------------------------------------------
#ifndef __driverRelay_H__
#define __driverRelay_H__
//-------------------------------------------------------------------

#include "Driver.hpp"

//-------------------------------------------------------------------
#ifndef NO_RELAY
#ifndef NO_MOTOR_LIGHT
class DriverRelay : public Driver
{
	private:
	
	public:
		DriverRelay(unsigned char inPortsNb);
		
	public:
		void begin() {}
		void beginPort(unsigned char inPort, int inPin);
};
#endif
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
