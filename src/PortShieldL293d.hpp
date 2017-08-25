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

/**This class is dedicated to the L293D shield for Arduino.
This shield have fix pins and use a specific version of the class AF_DCMotor to work.
This file is extracted from an old library called AF_Motor, discontinued and deprecated
because of a new hardware model of the shield working with I2C...
*/
class PortShieldL293d : public Port
{
	protected:
		/**Instance of the AF_Motor class.*/
		AF_DCMotor *pmotor;

	public:
		/**Default constructor.*/
		PortShieldL293d();
		
		/** Initialize the instance.
		@param inOutPort shield port number, 1 to 4. Use SHIELDL293D_PORT_M* defines.
		@param inSpeed Default speed.
		@param inFreq Frequencies for port number 1 & 2 are MOTOR12_64KHZ, MOTOR12_8KHZ, MOTOR12_2KHZ, MOTOR12_1KHZ,
			Frequencies for port number 3 & 4 are MOTOR34_64KHZ, MOTOR34_8KHZ, MOTOR34_1KHZ
		*/
		void begin(unsigned char inOutPort, uint8_t inSpeed, uint8_t inFreq);
		
		/**Sets the current speed of the port.
		@param inSpeed new current speed of the port.
		@return Previous speed.
		*/
		int SetSpeed(uint8_t inSpeed);
		
		/**Sets the current state of the port to PORT_LEFT.
		@param inDuration the pins will be activated to left for the given delay.
		*/
		void MoveLeftDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_RIGHT.
		@param inDuration the pins will be activated to right for the given delay.
		*/
		void MoveRightDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_STOP.	*/
		void MoveStop();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/**Internal data for print function. */
		unsigned char printedOutPort;
		/**Internal data for print function. */
		uint8_t printedSpeed;
		/** Print this port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printPort();
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
