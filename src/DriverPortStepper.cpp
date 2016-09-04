/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a stepper motor>
*************************************************************/

#include "Accessories.h"
#include "stepper.h"

#ifndef NO_STEPPER

DriverPortStepper::DriverPortStepper(uint8_t inId) : DriverPort(STEPPER, inId)
{
}

void DriverPortStepper::begin(uint8_t inPin1, uint8_t inPin2, uint8_t *inpSteps)
{
	this->begin(inPin1, inPin2, 255, 255, inpSteps);
}

void DriverPortStepper::begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, uint8_t *inpSteps)
{
	this->powerCommandPin[0] = inPin1;
	this->powerCommandPin[1] = inPin2;
	this->powerCommandPin[2] = inPin3;
	this->powerCommandPin[3] = inPin4;

	if (this->powerCommandPin[2] == 255)
		this->pMotor = new AccelStepper(this->powerCommandPin[0], this->powerCommandPin[1], inpSteps);
	else
		this->pMotor = new AccelStepper(this->powerCommandPin[0], this->powerCommandPin[1], this->powerCommandPin[2], this->powerCommandPin[3], inpSteps);
}

void DriverPortStepper::MovePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed(this->GetSpeed());
	this->pMotor->moveTo(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortStepper "));
	Serial.print(this->id);
	Serial.print(F(" MovePosition() "));
	Serial.print(F("to "));
	Serial.println(inPosition);
#endif
}

void DriverPortStepper::MoveRelativePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed(this->GetSpeed());
	this->pMotor->move(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortStepper "));
	Serial.print(this->id);
	Serial.print(F(" MoveRelativePosition() "));
	Serial.print(F("for "));
	Serial.print(inPosition);
	Serial.println(F(" steps."));
#endif
}
#endif