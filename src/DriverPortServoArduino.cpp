/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a servo on Arduino>
*************************************************************/

#include "Accessories.h"

#ifndef NO_SERVO

DriverPortServoArduino::DriverPortServoArduino(byte inId) : DriverPortServo(inId)
{
	this->pin = -1;
}

void DriverPortServoArduino::begin(int inPin, PORT_TYPE inType)
{
#ifdef DEBUG_MODE
	if (inType != ANALOG)
		Serial.println(F("Invalid pin type. Must be PWM pin!."));
#endif

	CHECKPIN(inPin, "DriverPortServoArduino::begin");
	this->pin = inPin;
	this->servo.attach(this->pin);
}

void DriverPortServoArduino::beginByAccessory(int inStartingPosition)
{
#ifdef DEBUG_MODE
	Serial.print(F("   DriverPortServoArduino "));
	Serial.print(this->pin, DEC);
	Serial.print(F(" beginByAccessory() "));
	Serial.print(F("Starting pos: "));
	Serial.println(inStartingPosition, DEC);
#endif

#ifdef DEBUG_MODE
	if (!this->servo.attached())
		Serial.println(F("Invalid servo beginByAccessory()."));
#endif

	this->servo.write(inStartingPosition);
}

void DriverPortServoArduino::MovePosition(unsigned long inDuration, int inEndPosition)
{
	CHECKPIN(this->pin, "DriverPortServoArduino::MovePosition");
#ifdef DEBUG_MODE
	Serial.print(F(" DriverPortServoArduino "));
	Serial.print(this->pin);
	Serial.print(F(" MovePosition() "));
	if (inDuration != -1)
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

int DriverPortServoArduino::GetPosition()
{
	return this->servo.read();
}
#endif
