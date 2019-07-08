#include "Accessories.h"

#ifndef NO_EXPANDER_74HC595

PortExpander74HC595::PortExpander74HC595() 
{
	this->psr = NULL;
}

byte PortExpander74HC595::begin(int inId, int inNumberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin)
{
	this->id = inId;

	this->numberOfShiftRegisters = inNumberOfShiftRegisters;
	this->psr = new ShiftRegister74HC595(inNumberOfShiftRegisters, serialDataPin, clockPin, latchPin);
	if (this->psr != NULL)
	{
		PortExpander::Add(this);
		Serial.println(F("Expander 74HC595 succeed"));
		return 1;
	}

	return 0;
}

void PortExpander74HC595::beginPin(int inPin, PIN_TYPE inType)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if (inType != OUTPUT)
		Serial.println(F("74HC595 : only output digital pins"));
#endif

	this->psr->set(inPin, LOW);
}

void PortExpander74HC595::digitalWrite(int inPin, int inValue)
{
	this->psr->set(inPin, inValue);
}

void PortExpander74HC595::analogWrite(int inPin, int inValue)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("74HC595 : only digital pins"));
#endif
	//	this->psr->analogWrite(inPin, inValue);
}

#endif
