//-------------------------------------------------------------------
#ifndef __driverPortL293d_H__
#define __driverPortL293d_H__
//-------------------------------------------------------------------

#if !defined(__AVR_ATmega32U4__)
#if defined VISUALSTUDIO
#define RELEASE		1
#define FORWARD		2
#define BACKWARD	3
class AF_DCMotor
{
public:
	AF_DCMotor(int, int) {}

	void run(int) {}
	void setSpeed(uint8_t) {}

	int pwmfreq;
};
#else
#include "AFMotor.hpp"
#endif
#include "DriverPort.hpp"

//-------------------------------------------------------------------

//-------------------------------------------------------------------

class DriverPortShieldL293d : public DriverPort
{
	protected:
		AF_DCMotor *pmotor;

	public:
		DriverPortShieldL293d(uint8_t inId, unsigned char inOutPort, uint8_t inSpeed, uint8_t inFreq);
		
		void begin(uint8_t inFreq);
		
		int SetSpeed(uint8_t inSpeed);
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};


//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
