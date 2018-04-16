/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a servo on Arduino>
*************************************************************/

#include "Accessories.h"

#ifndef NO_SERVO

PortServo::PortServo() : Port()
{
	this->pin = -1;
	this->SetPinType(DIGITAL);
}

void PortServo::begin(int inPin)
{
	CHECKPIN(Arduino_to_GPIO_pin(inPin), this->GetPinType(), "PortServo::begin");
	this->pin = inPin;
}

void PortServo::beginByAccessory(int inStartingPosition)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("  PortServo "));
	Serial.print(this->pin);
	Serial.print(F(" beginByAccessory() "));
	Serial.print(F("Starting pos: "));
	Serial.println(inStartingPosition, DEC);
#endif

	if (!this->servo.attached())
		this->servo.attach(this->pin);
	this->servo.write(inStartingPosition);
	this->servo.detach();
}

void PortServo::MovePosition(unsigned long inDuration, int inEndPosition)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("  PortServo "));
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
	if (!this->servo.attached())
		this->servo.attach(this->pin);
	this->servo.write(inEndPosition);
}

int PortServo::GetPosition()
{
	if (!this->servo.attached())
		this->servo.attach(this->pin);
	return this->servo.read();
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortServo::printPort()
{
	Serial.print(F("[PortServo pin:"));
	Serial.print(this->pin);
	Serial.print(F("]"));
}
#endif
#endif
