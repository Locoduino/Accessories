//-------------------------------------------------------------------
#ifndef __driverl293n_H__
#define __driverl293n_H__
//-------------------------------------------------------------------

#if !defined(__AVR_ATmega32U4__)

//-------------------------------------------------------------------

#ifndef NO_SHIELDL293D
#if defined VISUALSTUDIO
#define MOTOR12_1KHZ		1
#define MOTOR34_1KHZ		4
#endif

#define L293D_PORT_M1			0
#define L293D_PORT_M2			1
#define L293D_PORT_M3			2
#define L293D_PORT_M4			3

#define L293D_PORT_SERVO1		0
#define L293D_PORT_SERVO2		1

#define L293D_PORT_STEPPER12		0
#define L293D_PORT_STEPPER34		1
#define L293D_PORT_STEPPER1234		2

#define SERVO1_PIN		10
#define SERVO2_PIN		9

#define L293D_DEFAULTDURATION	100

class DriverPortServoBase;

class DriverShieldL293d : public Driver
{
	public:
		DriverShieldL293d();
		
	public:
		void begin();
		DriverPortShieldL293d *beginPortMotor(uint8_t inPort, uint8_t inFreq);
		DriverPortServoBase *beginPortServo(uint8_t inPort);
		//DriverPortStepper *beginPortStepper(uint8_t inPort);
};
#endif	 

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
