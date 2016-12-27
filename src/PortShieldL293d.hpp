//-------------------------------------------------------------------
#ifndef __PortShieldL293d_H__
#define __PortShieldL293d_H__
//-------------------------------------------------------------------

#if !defined(__AVR_ATmega32U4__)
#if defined VISUALSTUDIO
#include "AFMotor.h"
#else
#include "AFMotor.hpp"
#endif
#include "Port.hpp"

#define SHIELDL293D_PORT_M1			1
#define SHIELDL293D_PORT_M2			2
#define SHIELDL293D_PORT_M3			3
#define SHIELDL293D_PORT_M4			4

#define SHIELDL293D_SERVO1_PIN		10
#define SHIELDL293D_SERVO2_PIN		9

//-------------------------------------------------------------------

class PortShieldL293d : public Port
{
	protected:
		AF_DCMotor *pmotor;

	public:
		PortShieldL293d();
		
		void begin(unsigned char inOutPort, uint8_t inSpeed, uint8_t inFreq);
		
		int SetSpeed(uint8_t inSpeed);
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
#ifdef ACCESSORIES_PRINT_ACCESSORIES
		unsigned char printedOutPort;
		uint8_t printedSpeed;
		void printPort();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
