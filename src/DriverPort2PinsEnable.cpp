/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port with 2 pins and an Enable pin>
*************************************************************/

#include "Accessories.h"

DriverPort2PinsEnable::DriverPort2PinsEnable(uint8_t inId) : DriverPort2Pins(inId)
{
}

void DriverPort2PinsEnable::begin(int inPinA, int inPinB, int inPinEnable)
{
	this->DriverPort2Pins::begin(inPinA, inPinB);

	this->pinEnable = Arduino_to_GPIO_pin(inPinEnable);

	CHECKPIN(this->pinEnable, "DriverPort2PinsEnable::begin");

	pinMode2f(this->pinEnable, OUTPUT);
}

void DriverPort2PinsEnable::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPort2PinsEnable MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	digitalWrite2f(this->pinEnable, HIGH);
	digitalWrite2f(this->pinA, HIGH);
	digitalWrite2f(this->pinB, LOW);

	if (inDuration != 0)
	{
		delay(inDuration);

		digitalWrite2f(this->pinA, LOW);
		digitalWrite2f(this->pinB, LOW);
		digitalWrite2f(this->pinEnable, LOW);
	}

	this->state = PORT_LEFT;
}

void DriverPort2PinsEnable::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.print(F(" DriverPort2PinsEnable MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	digitalWrite2f(this->pinEnable, HIGH);
	digitalWrite2f(this->pinA, LOW);
	digitalWrite2f(this->pinB, HIGH);

	if (inDuration != 0)
	{
		delay(inDuration);

		digitalWrite2f(this->pinA, LOW);
		digitalWrite2f(this->pinB, LOW);
		digitalWrite2f(this->pinEnable, LOW);
	}

	this->state = PORT_RIGHT;
}

void DriverPort2PinsEnable::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(this->pinA);
	Serial.print(F(" / "));
	Serial.print(this->pinB);
	Serial.println(F(" DriverPort2PinsEnable MoveStop() "));
#endif

	digitalWrite2f(this->pinA, LOW);
	digitalWrite2f(this->pinB, LOW);
	digitalWrite2f(this->pinEnable, LOW);

	this->state = PORT_STOP;
}
