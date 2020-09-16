#include "Accessories.h"

#ifndef NO_EXPANDER_74HC595

template<uint8_t Size>
PortExpander74HC595<Size>::PortExpander74HC595()
{
	this->psr = NULL;
	this->numberOfShiftRegisters = Size;
}

template<uint8_t Size>
byte PortExpander74HC595<Size>::begin(int inId, int serialDataPin, int clockPin, int latchPin)
{
	this->id = inId;

	this->numberOfShiftRegisters = Size;
	this->psr = new ShiftRegister74HC595<Size>(serialDataPin, clockPin, latchPin);
	if (this->psr != NULL)
	{
		PortExpander::Add(this);
		Serial.println(F("Expander 74HC595 succeed"));
		return 1;
	}

	return 0;
}

template<uint8_t Size>
void PortExpander74HC595<Size>::beginPin(int inPin, PIN_TYPE inType)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if (inType != OUTPUT)
		Serial.println(F("74HC595 : only output digital pins"));
#endif

	this->psr->set(inPin, LOW);
}

template<uint8_t Size>
void PortExpander74HC595<Size>::digitalWrite(int inPin, int inValue)
{
	this->psr->set(inPin, inValue);
}

template<uint8_t Size>
void PortExpander74HC595<Size>::analogWrite(int inPin, int inValue)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("74HC595 : only digital pins"));
#endif
	//	this->psr->analogWrite(inPin, inValue);
}

#endif
