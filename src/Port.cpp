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
}
#else
void Port::CheckPinNb(int inPin, PIN_TYPE inType, const __FlashStringHelper *inFunc)
{
	int pin = inPin;
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
	int pin = -1;
	if (inType == UNDEFINED)
		inType = this->GetPinType();
	if (inType < ANALOG)
	{
		pin = Arduino_to_GPIO_pin(inPin);
		pinMode2f((GPIO_pin_t)pin, OUTPUT);
	}
	else
		pin = inPin;

	MovePin(pin, LOW, inType);

	return pin;
}

void Port::MovePin(int inPin, int inValue, PIN_TYPE inType) const
{
	if (inType == UNDEFINED)
		inType = this->GetPinType();

	CHECKPIN(inPin, inType, "Incorrect pin number in MovePin");

	if (inType < ANALOG)
		digitalWrite2f((GPIO_pin_t)inPin, this->MapValue(inValue, inType));
	else
		if (inValue == HIGH)
			analogWrite(inPin, this->MapValue(this->GetSpeed(), inType));
		else
			analogWrite(inPin, this->MapValue(0, inType));
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
	if (inType < ANALOG)
		Serial.print(GPIO_to_Arduino_pin((GPIO_pin_t)inPin));
	else
		Serial.print(inPin);
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
