#include "Accessories.h"

#ifndef NO_EXPANDER_SX1509

PortExpanderSX1509::PortExpanderSX1509() {}

byte PortExpanderSX1509::begin(int inId, byte inAddress)
{
	this->id = inId;
	if (this->sx1509.begin(inAddress) == 1)
	{
		PortExpander::Add(this);
		return 1;
	}
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("Expander SX1509 failed"));
#endif
	return 0;
}

void PortExpanderSX1509::beginPin(int inPin, PIN_TYPE inType)
{
	if (inType < ANALOG)
	{
		sx1509.pinMode(inPin, OUTPUT);
	}
	else
	{
		sx1509.pinMode(inPin, ANALOG_OUTPUT);
	}
}

void PortExpanderSX1509::digitalWrite(int inPin, int inValue)
{
	sx1509.digitalWrite(inPin, inValue);
}

void PortExpanderSX1509::analogWrite(int inPin, int inValue)
{
	sx1509.analogWrite(inPin, inValue);
}

#endif
