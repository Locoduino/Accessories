//-------------------------------------------------------------------
#ifndef __driverArduino_H__
#define __driverArduino_H__
//-------------------------------------------------------------------

#include "Driver.hpp"
#include "Chain.hpp"

//-------------------------------------------------------------------
#ifndef NO_ARDUINODRIVER

class DriverArduino : public Driver
{
	public:
		DriverArduino();

	public:
		void begin() {}

		// In Arduino ports, the pin is also the id of the port !
		DriverPortArduino *AddPortMotor(int inPin, PORT_TYPE inType = DIGITAL);
#ifndef NO_SERVO
		DriverPortServo *AddPortServo(int inPin, PORT_TYPE inType = ANALOG);
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
