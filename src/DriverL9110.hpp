//-------------------------------------------------------------------
#ifndef __driverL9910_H__
#define __driverL9910_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_L9110

class DriverPortServoBase;

#define L9110_PORT_A			0
#define L9110_PORT_B			1

class DriverL9110 : public Driver
{
	public:
		DriverL9110();
		
	public:
		void begin();
		DriverPort2Pins *beginPortMotor(uint8_t inPort, uint8_t inPinA, int inPinB);
		DriverPortServoBase *beginPortServo(uint8_t inPort, uint8_t inPin);
};
#endif	 

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
