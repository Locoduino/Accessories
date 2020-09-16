//-------------------------------------------------------------------
#ifndef __portexpanderPCA950506_H__
#define __portexpanderPCA950506_H__
//-------------------------------------------------------------------

#ifndef NO_EXPANDER_PCA9505_06

#include "PortExpander.hpp"
#include <Wire.h> // Include the I2C library (required)
#include "PCA9505_9506.h" // Include PCA9505_06 library

class PortExpanderPCA9505_06 : public PortExpander
{
	private:
		PCA9505_06 GPIO; // Create an SX1509 object to be used throughout

	public:
		byte begin(int inId, byte inAddress);
		void beginPin(int inPin, PIN_TYPE inType);
		void digitalWrite(int inPin, int inValue);
		void analogWrite(int inPin, int inValue);
		byte GetPinsNumber() { return 16;	}

	public:
		/**Default constructor.*/
		PortExpanderPCA9505_06();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
