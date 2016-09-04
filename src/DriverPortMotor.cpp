/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a relay>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR_LIGHT

DriverPortMotor::DriverPortMotor(uint8_t inId) : DriverPort(MOTOR_LIGHT, inId)
{
	this->pin = (GPIO_pin_t) DP_INVALID;
}

void DriverPortMotor::begin(int inPin)
{
	CHECKPIN(inPin, "DriverPortMotor::begin");
	this->pin = Arduino_to_GPIO_pin(inPin);

	pinMode2f(this->pin, OUTPUT);
	digitalWrite2f(this->pin, HIGH);
}

void DriverPortMotor::MoveLeftDir(unsigned long inDuration)
{
	CHECKPIN(this->pin, "DriverPortMotor::MoveLeftDir");
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortMotor "));
	Serial.print((int) this->pin);
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
	digitalWrite2f(this->pin, LOW);
}

void DriverPortMotor::MoveRightDir(unsigned long inDuration)
{
	CHECKPIN(this->pin, "DriverPortMotor::MoveRightDir");
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortMotor "));
	Serial.print((int) this->pin);
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
	digitalWrite2f(this->pin, LOW);
}

void DriverPortMotor::MoveStop()
{
	CHECKPIN(this->pin, "DriverPortMotor::MoveStop");
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F(" DriverPortMotor MoveStop()"));
#endif
	this->state = PORT_STOP;
	digitalWrite2f(this->pin, HIGH);
}

#endif