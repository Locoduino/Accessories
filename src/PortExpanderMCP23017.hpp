//-------------------------------------------------------------------
#ifndef __portexpanderMCP23017_H__
#define __portexpanderMCP23017_H__
//-------------------------------------------------------------------

#ifndef NO_EXPANDER_MCP23017

#include "PortExpander.hpp"
#include <Wire.h> // Include the I2C library (required)
#include "Adafruit_MCP23017.h" // Include MCP23017 library

class PortExpanderMCP23017 : public PortExpander
{
	private:
		Adafruit_MCP23017 GPIO; // Create an MCP23017 object to be used throughout

	public:
		byte begin(int inId, byte inAddress);
		void beginPin(int inPin, PIN_TYPE inType);
		void digitalWrite(int inPin, int inValue);
		void analogWrite(int inPin, int inValue);
		byte GetPinsNumber() { return 16;	}

	public:
		/**Default constructor.*/
		PortExpanderMCP23017();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
