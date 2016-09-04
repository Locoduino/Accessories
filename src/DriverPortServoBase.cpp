/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a servo on Arduino>
*************************************************************/

#include "Accessories.h"

#ifndef NO_SERVO

DriverPortServoBase::DriverPortServoBase(uint8_t inId) : DriverPortServo(inId)
{
	this->pin = -1;
	//this->servo.pin = -1;
	//this->servo.servoIndex = -1;
}

void DriverPortServoBase::begin(int inPin, PORT_TYPE inType)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if (inType != ANALOG)
		Serial.println(F("Invalid pin type. Must be PWM pin!."));
#endif

	CHECKPIN(inPin, "DriverPortServoBase::begin");
	this->pin = inPin;
	this->servo.attach(this->pin);
}

void DriverPortServoBase::beginByAccessory(int inStartingPosition)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("   DriverPortServoBase "));
	Serial.print(this->pin, DEC);
	Serial.print(F(" beginByAccessory() "));
	Serial.print(F("Starting pos: "));
	Serial.println(inStartingPosition, DEC);
#endif

#ifdef ACCESSORIES_DEBUG_MODE
	if (!this->servo.attached())
		Serial.println(F("Invalid servo beginByAccessory()."));
#endif

	this->servo.write(inStartingPosition);
}

void DriverPortServoBase::MovePosition(unsigned long inDuration, int inEndPosition)
{
	CHECKPIN(this->pin, "DriverPortServoBase::MovePosition");
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortServoBase "));
	Serial.print(this->pin);
	Serial.print(F(" MovePosition() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.print(F("ms "));
	}
	Serial.print(F("to angle "));
	Serial.println(inEndPosition);
#endif
	this->servo.write(inEndPosition);
}

int DriverPortServoBase::GetPosition()
{
	return this->servo.read();
}
#endif
