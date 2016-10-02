//-------------------------------------------------------------------
#ifndef __driverl293d_H__
#define __driverl293d_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_L293D

#define L293D_PORT_OUT12		0
#define L293D_PORT_OUT34		1

class DriverL293d : public Driver
{
	private:
	
	public:
		DriverL293d();
		
	public:
		inline void begin() {}
		DriverPort2PinsEnable *beginPortMotor(uint8_t inPort, int inPinA, int inPinB, int inPinEnable);
		DriverPortStepper *beginPortStepper(int inPinA, int inPinB, int inPinC, int inPinD, uint8_t *inpSteps = NULL);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
