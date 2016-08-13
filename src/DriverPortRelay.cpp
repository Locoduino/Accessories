/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a relay>
*************************************************************/

#include "Accessories.h"

#ifndef NO_RELAY

DriverPortRelay::DriverPortRelay(byte inId) : DriverPort(MOTOR_LIGHT, inId)
{
	this->pin = (GPIO_pin_t) DP_INVALID;
}

void DriverPortRelay::begin(int inPin)
{
	CHECKPIN(inPin, "DriverPortRelay::begin");
	this->pin = Arduino_to_GPIO_pin(inPin);

	pinMode2f(this->pin, OUTPUT);
	digitalWrite2f(this->pin, HIGH);
}

void DriverPortRelay::MoveLeftDir(unsigned long inDuration)
{
	CHECKPIN(this->pin, "DriverPortRelay::MoveLeftDir");
#ifdef DEBUG_MODE
	Serial.print(F(" DriverPortRelay "));
	Serial.print((int) this->pin);
	Serial.print(F(" MoveLeftDir() "));
	if (inDuration != -1)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->state = PORT_LEFT;
	digitalWrite2f(this->pin, LOW);
}

void DriverPortRelay::MoveRightDir(unsigned long inDuration)
{
	CHECKPIN(this->pin, "DriverPortRelay::MoveRightDir");
#ifdef DEBUG_MODE
	Serial.print(F(" DriverPortRelay "));
	Serial.print((int) this->pin);
	Serial.print(F(" MoveRightDir() "));
	if (inDuration != -1)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->state = PORT_RIGHT;
	digitalWrite2f(this->pin, LOW);
}

void DriverPortRelay::MoveStop()
{
	CHECKPIN(this->pin, "DriverPortRelay::MoveStop");
#ifdef DEBUG_MODE
	Serial.println(F(" DriverPortRelay MoveStop()"));
#endif
	this->state = PORT_STOP;
	digitalWrite2f(this->pin, HIGH);
}

#endif