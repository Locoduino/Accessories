/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for the Arduino>
*************************************************************/

#include "Accessories.h"

#ifndef NO_ARDUINODRIVER

uint8_t localHigh = HIGH;
uint8_t localLow = LOW;

DriverPortArduino::DriverPortArduino(uint8_t inId) : DriverPort(MOTOR_LIGHT, inId)
{
	this->pin = (GPIO_pin_t) DP_INVALID;
	this->portType = DIGITAL;
}

int DriverPortArduino::SetSpeed(int inSpeed)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if (inSpeed != 0 && inSpeed != 255 && (this->portType == DIGITAL || this->portType == DIGITAL_INVERTED))
	{
		Serial.print(F(" DriverPortArduino "));
		Serial.print(GPIO_to_Arduino_pin(this->pin));
		Serial.print(F(" SetSpeed() error : The port must be ANALOG or ANALOG_INVERTED !"));
	}
#endif
	return DriverPort::SetSpeed(inSpeed);
}

int DriverPortArduino::GetAnalogValue(int inValue) const
{
	if (this->portType == ANALOG_INVERTED)
		return 255 - inValue;

	return inValue;
}

void DriverPortArduino::begin(int inPin, PORT_TYPE inType)
{
	CHECKPIN(inPin, "DriverPortArduino::begin");
	this->pin = Arduino_to_GPIO_pin(inPin);
	this->portType = inType;
	if (portType == DIGITAL_INVERTED)
	{
		localLow = HIGH;
		localHigh = LOW;
	}

	if (this->GetPortType() == DIGITAL || this->GetPortType() == DIGITAL_INVERTED)
	{
		pinMode2f(this->pin, OUTPUT);
		digitalWrite2f(this->pin, localLow);
	}
	else
	{
		analogWrite(inPin, GetAnalogValue(0));
	}
}

void DriverPortArduino::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortArduino "));
	Serial.print(GPIO_to_Arduino_pin(this->pin));
	Serial.print(F(" MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.print(F("ms"));
	}
	Serial.print(F(" at speed "));
	Serial.print(this->GetSpeed());
	Serial.print(F(" (real speed from pin: "));
	Serial.print(GetAnalogValue(this->GetSpeed()));
	Serial.println(F(")"));
#endif
	this->state = PORT_LEFT;
	if (this->GetPortType() == DIGITAL || this->GetPortType() == DIGITAL_INVERTED)
		digitalWrite2f(this->pin, localHigh);
	else
		analogWrite(GPIO_to_Arduino_pin(this->pin), GetAnalogValue(this->GetSpeed()));
}

void DriverPortArduino::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortArduino "));
	Serial.print(GPIO_to_Arduino_pin(this->pin));
	Serial.print(F(" MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.print(F("ms"));
	}
	Serial.print(F(" at speed "));
	Serial.println(GetAnalogValue(this->GetSpeed()));
#endif
	this->state = PORT_RIGHT;
	if (this->GetPortType() == DIGITAL || this->GetPortType() == DIGITAL_INVERTED)
		digitalWrite2f(this->pin, localHigh);
	else
		analogWrite(GPIO_to_Arduino_pin(this->pin), GetAnalogValue(this->GetSpeed()));
}

void DriverPortArduino::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortArduino "));
	Serial.print(GPIO_to_Arduino_pin(this->pin));
	Serial.println(F(" MoveStop()"));
#endif
	this->state = PORT_STOP;
	if (this->GetPortType() == DIGITAL || this->GetPortType() == DIGITAL_INVERTED)
		digitalWrite2f(this->pin, localLow);
	else
		analogWrite(GPIO_to_Arduino_pin(this->pin), GetAnalogValue(0));
}

#endif