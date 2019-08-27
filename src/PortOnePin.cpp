/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port with only one pin>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR_LIGHT

PortOnePin::PortOnePin()
{
	this->pin = -1;
}

void PortOnePin::begin(int inPin, PIN_TYPE inType)
{
	this->SetPinType(inType);
	this->pin = this->beginPin(inPin);
}

int PortOnePin::SetSpeed(int inSpeed)
{
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	if (inSpeed != 0 && inSpeed != 255 && this->GetPinType() < ANALOG)
	{
		Serial.print(F(" PortOnePin "));
		Port::printPortPin(this->pin, this->GetPinType());
		Serial.print(F(" SetSpeed() error : The port must be ANALOG or ANALOG_INVERTED !"));
	}
#endif
	return Port::SetSpeed(inSpeed);
}

void PortOnePin::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F(" PortOnePin "));
	Port::printPortPin(this->pin, this->GetPinType());
	Serial.print(F(" MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.print(F("ms"));
	}
	Serial.print(F(" at speed "));
	Serial.println(this->speed);
#endif
	this->SetPortState(PORT_LEFT);
	this->MovePin(this->pin, HIGH);
}

void PortOnePin::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F(" PortOnePin "));
	Port::printPortPin(this->pin, this->GetPinType());
	Serial.print(F(" MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->SetPortState(PORT_RIGHT);
	this->MovePin(this->pin, HIGH);
}

void PortOnePin::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F(" PortOnePin "));
	Port::printPortPin(this->pin, this->GetPinType());
	Serial.println(F(" MoveStop()"));
#endif
	this->SetPortState(PORT_STOP);
	this->MovePin(this->pin, LOW);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortOnePin::printPort()
{
	Serial.print(F("[PortOnePin pin:"));
	Port::printPortPin(this->pin, this->GetPinType());
	Serial.print(F("]"));
}
#endif

#endif

