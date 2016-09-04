/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a LMD18200>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR_LIGHT

DriverPortLMD18200::DriverPortLMD18200(uint8_t inId) : DriverPort(MOTOR_LIGHT, inId)
{
	this->pinPWM = (GPIO_pin_t) DP_INVALID;
}

void DriverPortLMD18200::begin(int inPinPWM, int inPinDir, int inPinBrake)
{
	CHECKPIN(inPinPWM, "DriverPortLMD18200::begin");
	CHECKPIN(inPinDir, "DriverPortLMD18200::begin");
	CHECKPIN(inPinBrake, "DriverPortLMD18200::begin");
	this->pinPWM = inPinPWM;
	this->pinDir = Arduino_to_GPIO_pin(inPinDir);
	this->pinBrake = Arduino_to_GPIO_pin(inPinBrake);

	pinMode(this->pinPWM, OUTPUT);
	pinMode2f(this->pinDir, OUTPUT);
	pinMode2f(this->pinBrake, OUTPUT);
}

void DriverPortLMD18200::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortLMD18200 MoveLeftDir() "));
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

	analogWrite(this->pinPWM, this->GetSpeed());
	digitalWrite2f(this->pinDir, LOW);
	digitalWrite2f(this->pinBrake, LOW);
}

void DriverPortLMD18200::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortLMD18200 MoveRightDir() "));
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
	analogWrite(this->pinPWM, this->GetSpeed());
	digitalWrite2f(this->pinDir, HIGH);
	digitalWrite2f(this->pinBrake, LOW);
}

void DriverPortLMD18200::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F(" DriverPortLMD18200 MoveStop()"));
#endif
	this->state = PORT_STOP;
	digitalWrite2f(this->pinBrake, HIGH);
}

#endif