/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for L298n>
*************************************************************/

#include "Accessories.h"

#ifndef NO_L298N

DriverPortL298n::DriverPortL298n(byte inId) : DriverPort(MOTOR_LIGHT, inId)
{
}

void DriverPortL298n::begin(int inPinA, int inPinB)
{
	this->pinA = Arduino_to_GPIO_pin(inPinA);
	this->pinB = Arduino_to_GPIO_pin(inPinB);

	CHECKPIN(this->pinA, "DriverPortL298n::begin");
	CHECKPIN(this->pinB, "DriverPortL298n::begin");

	pinMode2f(this->pinA, OUTPUT);
	pinMode2f(this->pinB, OUTPUT);
}

uint8_t DriverPortL298n::SetSpeed(uint8_t inSpeed)
{
	return 0;
}

void DriverPortL298n::MoveLeftDir(unsigned long inDuration)
{
	CHECKPIN(this->pinA, "DriverPortL298n::MoveLeftDir");
	CHECKPIN(this->pinB, "DriverPortL298n::MoveLeftDir");
#ifdef DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPortL298n MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	digitalWrite2f(this->pinA, HIGH);
	digitalWrite2f(this->pinB, LOW);

	if (inDuration != 0)
	{
		delay(inDuration);

		digitalWrite2f(this->pinA, LOW);
		digitalWrite2f(this->pinB, LOW);
	}

	this->state = PORT_LEFT;
}

void DriverPortL298n::MoveRightDir(unsigned long inDuration)
{
	CHECKPIN(this->pinA, "DriverPortL298n::MoveRightDir");
	CHECKPIN(this->pinB, "DriverPortL298n::MoveRightDir");
#ifdef DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPortL298n MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	digitalWrite2f(this->pinA, LOW);
	digitalWrite2f(this->pinB, HIGH);

	if (inDuration != 0)
	{
		delay(inDuration);

		digitalWrite2f(this->pinA, LOW);
		digitalWrite2f(this->pinB, LOW);
	}

	this->state = PORT_RIGHT;
}

void DriverPortL298n::MoveStop()
{
	CHECKPIN(this->pinA, "DriverPortL298n::MoveStop");
	CHECKPIN(this->pinB, "DriverPortL298n::MoveStop");

	digitalWrite2f(this->pinA, LOW);
	digitalWrite2f(this->pinB, LOW);

	this->state = PORT_STOP;
}
#endif
