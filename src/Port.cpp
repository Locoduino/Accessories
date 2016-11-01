/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Port>
*************************************************************/

#include "Accessories.h"

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
void Port::CheckPinNb(int inPin, const char *inFunc)
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

Port::Port(PORTTYPE inType)
{
	this->type = inType;
	this->pinType = DIGITAL;
	this->state = PORT_STOP;
	this->speed = DEFAULTSPEED;
}

int Port::MapValue(int inValue, PIN_TYPE inType) const
{
	if (inType == UNDEFINED)
		inType = this->pinType;

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
		inType = this->pinType;
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
		inType = this->pinType;

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
		
	return this->GetState();
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

