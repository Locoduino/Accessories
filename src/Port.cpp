/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Port>
*************************************************************/

#include "Accessories.h"

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
void Port::CheckPinNb(int inPin, PIN_TYPE inType, const char *inFunc)
{
	int pin = inPin;

#ifndef NO_EXPANDER
	// Check for expander id validity and pin number
	unsigned long expanderId = 0;
	if (GPIO_to_Arduino_pin((GPIO_pin_t)inPin) == -1)
		expanderId = EXPANDER_PORT_EXPID(inPin);

	if (expanderId > 0)
	{
		PortExpander::CheckExpanderId(expanderId);
	}
	else
	{
#endif
		if (inType < ANALOG)
			pin = GPIO_to_Arduino_pin((GPIO_pin_t)inPin);

		if (pin <= 0 || pin >= NUM_DIGITAL_PINS)
		{
			if (pin < A0 || pin > A7)
			{
				Serial.print(F("Pin "));
				Serial.print(inPin);
				Serial.print(F(" error in "));
				Serial.println(inFunc);
			}
		}
#ifndef NO_EXPANDER
	}
#endif
}

#else
void Port::CheckPinNb(int inPin, PIN_TYPE inType, const __FlashStringHelper* inFunc)
{
	int pin = inPin;

#ifndef NO_EXPANDER
	// Check for expander id validity and pin number
	unsigned long expanderId = 0;
	if (GPIO_to_Arduino_pin((GPIO_pin_t)inPin) == -1)
		expanderId = EXPANDER_PORT_EXPID(inPin);

	if (expanderId > 0)
	{
		PortExpander::CheckExpanderId(expanderId);
	}
	else
	{
#endif
		if (inType < ANALOG)
			pin = GPIO_to_Arduino_pin((GPIO_pin_t)inPin);

		if (pin <= 0 || pin >= NUM_DIGITAL_PINS)
		{
			if (pin < A0 || pin > A7)
			{
				Serial.print(F("Pin "));
				Serial.print(inPin);
				Serial.print(F(" error in "));
				Serial.println(inFunc);
			}
		}
#ifndef NO_EXPANDER
	}
#endif
}
#endif
#endif

Port::Port()
{
	this->SetPinType(DIGITAL);
	this->SetPortState(PORT_STOP);
	this->speed = DEFAULTSPEED;
}

int Port::MapValue(int inValue, PIN_TYPE inType) const
{
	if (inType == UNDEFINED)
		inType = this->GetPinType();

	if (inType == ANALOG_INVERTED)
		return 255 - inValue;

	if (inType == DIGITAL_INVERTED)
	{
		if (inValue == LOW) 
			return HIGH;
		return LOW;
	}

	return inValue;
}

int Port::beginPin(int inPin, PIN_TYPE inType) const
{
	int pin = inPin;
	if (inType == UNDEFINED)
		inType = this->GetPinType();

#ifndef NO_EXPANDER
	// Check for expander id validity and pin number
	unsigned long expanderId = 0;
	if (GPIO_to_Arduino_pin((GPIO_pin_t)inPin) == -1)
		expanderId = EXPANDER_PORT_EXPID(inPin);

	if (expanderId > 0)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		PortExpander::CheckExpanderId(expanderId);
#endif
		int pinExp = EXPANDER_PORT_PIN(inPin);
		PortExpander::beginPin(pinExp, expanderId, inType);
	}
	else
	{
#endif

		if (inType < ANALOG)
		{
			pin = Arduino_to_GPIO_pin(inPin);
			pinMode2f((GPIO_pin_t) pin, OUTPUT);
		}
#ifndef NO_EXPANDER
	}
#endif

	MovePin(pin, LOW, inType);

	return pin;
}

void Port::MovePin(int inPin, int inValue, PIN_TYPE inType) const
{
	if (inType == UNDEFINED)
		inType = this->GetPinType();

	CHECKPIN(inPin, inType, "Incorrect pin number in MovePin");

#ifndef NO_EXPANDER
	unsigned long expanderId = 0;
	if (GPIO_to_Arduino_pin((GPIO_pin_t)inPin) == -1)
		expanderId = EXPANDER_PORT_EXPID(inPin);

	if (expanderId > 0)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		PortExpander::CheckExpanderId(expanderId);
#endif
		int pinExp = EXPANDER_PORT_PIN(inPin);
		if (inType < ANALOG)
			PortExpander::digitalWrite(pinExp, expanderId, this->MapValue(inValue, inType));
		else
		{
			if (inValue == HIGH)
				PortExpander::analogWrite(pinExp, expanderId, this->MapValue(this->GetSpeed(), inType));
			else
				PortExpander::analogWrite(pinExp, expanderId, this->MapValue(0, inType));
		}
	}
	else
	{
#endif
		if (inType < ANALOG)
			digitalWrite2f((GPIO_pin_t)inPin, this->MapValue(inValue, inType));
		else
			if (inValue == HIGH)
				analogWrite(inPin, this->MapValue(this->GetSpeed(), inType));
			else
				analogWrite(inPin, this->MapValue(0, inType));
#ifndef NO_EXPANDER
}
#endif
}

int Port::SetSpeed(int inSpeed)
{
	uint8_t oldSpeed = this->speed;
	this->speed = inSpeed;
	return oldSpeed;
}

PORT_STATE Port::MoveToggle(unsigned long inDuration)
{
	if (this->IsLeftDir())
	{
		MoveRightDir(inDuration);
	}
	else
	{
		MoveLeftDir(inDuration);
	}
		
	return this->GetPortState();
}

void Port::MoveLeftDir(unsigned long inDuration, int inSpeed)
{
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = this->SetSpeed(inSpeed);
	this->MoveLeftDir(inDuration);
	if (oldSpeed != -1)
		this->SetSpeed(oldSpeed);
}

void Port::MoveRightDir(unsigned long inDuration, int inSpeed)
{
	int oldSpeed = -1;
	if (inSpeed >= 0)
		oldSpeed = this->SetSpeed(inSpeed);
	this->MoveRightDir(inDuration);
	if (oldSpeed != -1)
		this->SetSpeed(oldSpeed);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void Port::printPort()
{
}

void Port::printPortPin(int inPin, PIN_TYPE inType)
{
	Serial.print(" ");
#ifndef NO_EXPANDER
	unsigned long expanderId = 0;
	if (GPIO_to_Arduino_pin((GPIO_pin_t)inPin) == -1)
			expanderId = EXPANDER_PORT_EXPID(inPin);

	if (expanderId > 0)
	{
		Serial.print(expanderId);
		Serial.print("/");
		Serial.print(EXPANDER_PORT_PIN(inPin));
	}
	else
#endif

	{
		if (inType < ANALOG)
			Serial.print(GPIO_to_Arduino_pin((GPIO_pin_t)inPin));
		else
			Serial.print(inPin);
	}

	Serial.print(" (");

	switch (inType)
	{
	case UNDEFINED:			Serial.print("UNDEFINED)"); break;
	case DIGITAL:			Serial.print("DIGITAL)"); break;
	case DIGITAL_INVERTED:	Serial.print("DIGITAL_INVERTED)"); break;
	case ANALOG:			Serial.print("ANALOG)"); break;
	case ANALOG_INVERTED:	Serial.print("ANALOG_INVERTED)"); break;
	}
}
#endif
