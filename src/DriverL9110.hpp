//-------------------------------------------------------------------
#ifndef __driverL9910_H__
#define __driverL9910_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_L9110

class DriverPortServoBase;

class DriverL9110 : public Driver
{
	public:
		DriverL9110();
		
	public:
		void begin();
		DriverPort2Pins *beginPortMotor(uint8_t inPinA, int inPinB);
		DriverPortServoBase *beginPortServo(uint8_t inPin);
};
#endif	 

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
