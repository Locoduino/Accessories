/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Port for a stepper motor>
*************************************************************/

#include "Accessories.h"

#ifndef NO_STEPPER

PortStepper::PortStepper()
{
}

void PortStepper::begin(uint8_t inPin1, uint8_t inPin2, PIN_TYPE inType, uint8_t *inpSteps)
{
	this->begin(inPin1, inPin2, 255, 255, inType, inpSteps);
}

void PortStepper::begin(uint8_t inPin1, uint8_t inPin2, uint8_t inPin3, uint8_t inPin4, PIN_TYPE inType, uint8_t *inpSteps)
{
	// Check ANALOG because stepper do not use GPIO_t ...
	CHECKPIN(inPin1, ANALOG, "PortStepper::begin");
	CHECKPIN(inPin2, ANALOG, "PortStepper::begin");
	if (inPin3 == 255)
		this->pMotor = new LocoStepper(inPin1, inPin2, inpSteps);
	else
	{
		CHECKPIN(inPin3, ANALOG, "PortStepper::begin");
		CHECKPIN(inPin4, ANALOG, "PortStepper::begin");
		this->pMotor = new LocoStepper(inPin1, inPin2, inPin3, inPin4, inpSteps);
	}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	this->printPin1 = inPin1;
	this->printPin2 = inPin2;
	this->printPin3 = inPin3;
	this->printPin4 = inPin4;

	this->printType = inType;
	this->printpSteps = inpSteps;
#endif
}

void PortStepper::MovePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed((float)this->GetSpeed());
	this->pMotor->moveTo(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortStepper MovePosition() "));
	Serial.print(F("to "));
	Serial.println(inPosition);
#endif
}

void PortStepper::MoveRelativePosition(unsigned long inDuration, int inPosition)
{
	this->pMotor->setSpeed((float)this->GetSpeed());
	this->pMotor->move(inPosition);

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortStepper MoveRelativePosition() "));
	Serial.print(F("for "));
	Serial.print(inPosition);
	Serial.println(F(" steps."));
#endif
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortStepper::printPort()
{
	byte count = 2;

	Serial.print(F("[PortStepper pin1:"));
	Port::printPortPin(this->printPin1, DIGITAL);
	Serial.print(F(" pin2:"));
	Port::printPortPin(this->printPin2, DIGITAL);
	if (this->printPin3 != 255)
	{
		Serial.print(F(" pin3:"));
		Port::printPortPin(this->printPin3, DIGITAL);
	}
	if (this->printPin4 != 255)
	{
		Serial.print(F(" pin4:"));
		Port::printPortPin(this->printPin4, DIGITAL);
		count = 4;
	}
	Serial.print(F(" steps:"));
	for (int i = 0; i < count; i++)
	{
		if (i > 0)
			Serial.print("/");
		Serial.print(this->printpSteps[i], DEC);
	}

	Serial.print(F("]"));
}
#endif
#endif