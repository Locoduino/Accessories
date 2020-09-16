#include "Accessories.h"

#ifndef NO_EXPANDER_MCP23017

PortExpanderMCP23017::PortExpanderMCP23017() {}

byte PortExpanderMCP23017::begin(int inId, byte inAddress)
{
	this->id = inId;
	this->GPIO.begin(inAddress);
	PortExpander::Add(this);
	return 1;
}

void PortExpanderMCP23017::beginPin(int inPin, PIN_TYPE inType)
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

void PortExpanderMCP23017::digitalWrite(int inPin, int inValue)
{
	this->GPIO.digitalWrite(inPin, inValue);
}

void PortExpanderMCP23017::analogWrite(int inPin, int inValue)
{
	//this->GPIO.analogWrite(inPin, inValue);
}

#endif
