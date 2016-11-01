/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a relay>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR_LIGHT

PortOnePin::PortOnePin() : Port(MOTOR_LIGHT)
{
	this->pin = -1;
}

void PortOnePin::begin(int inPin, PIN_TYPE inType)
{
	this->pinType = inType;
	this->pin = this->beginPin(inPin);
}

int PortOnePin::SetSpeed(int inSpeed)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if (inSpeed != 0 && inSpeed != 255 && this->pinType < ANALOG)
	{
		Serial.print(F(" PortOnePin "));
		Serial.print(this->GetPin());
		Serial.print(F(" SetSpeed() error : The port must be ANALOG or ANALOG_INVERTED !"));
	}
#endif
	return Port::SetSpeed(inSpeed);
}

void PortOnePin::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortOnePin "));
	Serial.print(this->GetPin());
	Serial.print(F(" MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->state = PORT_LEFT;
	this->MovePin(this->pin, HIGH);
}

void PortOnePin::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortOnePin "));
	Serial.print(this->GetPin());
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
	this->state = PORT_RIGHT;
	this->MovePin(this->pin, HIGH);
}

void PortOnePin::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F(" PortOnePin MoveStop()"));
#endif
	this->state = PORT_STOP;
	this->MovePin(this->pin, LOW);
}

#endif