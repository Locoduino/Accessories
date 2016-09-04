/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for L298n>
*************************************************************/

#include "Accessories.h"

#ifndef NO_L298N

DriverPort2Pins::DriverPort2Pins(uint8_t inId) : DriverPort(MOTOR_LIGHT, inId)
{
}

void DriverPort2Pins::begin(int inPinA, int inPinB)
{
	this->pinA = Arduino_to_GPIO_pin(inPinA);
	this->pinB = Arduino_to_GPIO_pin(inPinB);

	CHECKPIN(this->pinA, "DriverPort2Pins::begin");
	CHECKPIN(this->pinB, "DriverPort2Pins::begin");

	pinMode2f(this->pinA, OUTPUT);
	pinMode2f(this->pinB, OUTPUT);
}

void DriverPort2Pins::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPort2Pins MoveLeftDir() "));
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

void DriverPort2Pins::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPort2Pins MoveRightDir() "));
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

void DriverPort2Pins::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.println(F(" DriverPort2Pins MoveStop() "));
#endif

	digitalWrite2f(this->pinA, LOW);
	digitalWrite2f(this->pinB, LOW);

	this->state = PORT_STOP;
}
#endif
