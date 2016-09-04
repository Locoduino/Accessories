//-------------------------------------------------------------------
#ifndef __driverl298d_H__
#define __driverl298d_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_L298N
// Old style, only for compatibility
#define L298N_PORT_OUT1			0
#define L298N_PORT_OUT2			1

#define L298N_PORT_OUT12		0
#define L298N_PORT_OUT34		1

class DriverL298n : public Driver
{
	private:
	
	public:
		DriverL298n();
		
	public:
		inline void begin() {}
		DriverPort2Pins *beginPortMotor(uint8_t inPort, int inPinA, int inPinB);
//		DriverPortStepper *beginPortStepper(uint8_t inPort, int inPinA, int inPinB);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
