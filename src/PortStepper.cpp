/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Port for a stepper motor>
*************************************************************/

#include "Accessories.h"

#ifndef NO_STEPPER

PortStepper::PortStepper() : Port(STEPPER)
{
}

void PortStepper::begin(uint8_t inPin1, uint8_t inPin2, PIN_TYPE inType, uint8_t *inpSteps)
{
	this->begin(inPin1, inPin2, 255, 255, inType, inpSteps);
}

void PortStepper::begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, PIN_TYPE inType, uint8_t *inpSteps)
{
	CHECKPIN(inPin1, DIGITAL, "PortStepper::begin");
	CHECKPIN(inPin2, DIGITAL, "PortStepper::begin");
	if (inPin3 == 255)
		this->pMotor = new LocoStepper(inPin1, inPin2, inpSteps);
	else
	{
		CHECKPIN(inPin3, DIGITAL, "PortStepper::begin");
		CHECKPIN(inPin4, DIGITAL, "PortStepper::begin");
		this->pMotor = new LocoStepper(inPin1, inPin2, inPin3, inPin4, inpSteps);
	}
}

void PortStepper::MovePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed(this->GetSpeed());
	this->pMotor->moveTo(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortStepper MovePosition() "));
	Serial.print(F("to "));
	Serial.println(inPosition);
#endif
}

void PortStepper::MoveRelativePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed(this->GetSpeed());
	this->pMotor->move(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortStepper MoveRelativePosition() "));
	Serial.print(F("for "));
	Serial.print(inPosition);
	Serial.println(F(" steps."));
#endif
}
#endif