#include "Accessories.h"

#ifndef NO_EXPANDER_PCA9505_06

PortExpanderPCA9505_06::PortExpanderPCA9505_06() {}

byte PortExpanderPCA9505_06::begin(int inId, byte inAddress)
{
	this->id = inId;
	this->GPIO.begin(inAddress);
	PortExpander::Add(this);
	return 1;
}

void PortExpanderPCA9505_06::beginPin(int inPin, PIN_TYPE inType)
{
	if (inType < ANALOG)
	{
		this->GPIO.pinMode(inPin, OUTPUT);
	}
	else
	{
		this->GPIO.pinMode(inPin, ANALOG_OUTPUT);
	}
}

void PortExpanderPCA9505_06::digitalWrite(int inPin, int inValue)
{
	this->GPIO.digitalWrite(inPin, inValue);
}

void PortExpanderPCA9505_06::analogWrite(int inPin, int inValue)
{
	//this->GPIO.analogWrite(inPin, inValue);
}

#endif
