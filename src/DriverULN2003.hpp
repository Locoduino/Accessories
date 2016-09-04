//-------------------------------------------------------------------
#ifndef __driverULN2003_H__
#define __driverULN2003_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_ULN2003

class DriverULN2003 : public Driver
{
	private:
	
	public:
		DriverULN2003();
		
	public:
		inline void begin() {}
#ifndef NO_MOTOR_LIGHT
		DriverPortMotor *beginPortMotor(uint8_t inPort, int inPin);
#endif

#ifndef NO_SERVO
		DriverPortServo *beginPortServo(uint8_t inPort, int inPin);
#endif

#ifndef NO_STEPPER
		DriverPortStepper *beginPortStepper(uint8_t inPort, int inPin1, int inPin2, uint8_t *inpSteps = NULL);
		DriverPortStepper *beginPortStepper(uint8_t inPort, int inPin1, int inPin2, int inPin3, int inPin4, uint8_t *inpSteps = NULL);
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
