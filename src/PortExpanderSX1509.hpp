//-------------------------------------------------------------------
#ifndef __portexpandersx1509_H__
#define __portexpandersx1509_H__
//-------------------------------------------------------------------

#ifndef NO_EXPANDER_SX1509

#include "PortExpander.hpp"
#include <Wire.h> // Include the I2C library (required)
#include "SparkFunSX1509.h" // Include SX1509 library

class PortExpanderSX1509 : public PortExpander
{
	private:
		SX1509 sx1509; // Create an SX1509 object to be used throughout

	public:
		byte begin(int inId, byte inAddress);
		void beginPin(int inPin, PIN_TYPE inType);
		void digitalWrite(int inPin, int inValue);
		void analogWrite(int inPin, int inValue);
		byte GetPinsNumber() { return 16;	}

	public:
		/**Default constructor.*/
		PortExpanderSX1509();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
