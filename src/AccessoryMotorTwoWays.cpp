/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a two ways motorized accessory>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR
#ifndef NO_MOTORTWOWAYS

void AccessoryMotorTwoWays::beginTwoWays(Port *inpPort, unsigned long inIdRight, unsigned long inIdLeft, int speed, unsigned long inDurationMilli)
{
	this->AccessoryMotor::begin(inpPort, inIdRight, speed, inDurationMilli);
	this->AddMovingPosition(inIdLeft, LEFT);
}

ACC_STATE AccessoryMotorTwoWays::MoveToggle(unsigned long inDuration, int inSpeed)
{
	if (this->GetState() == LEFT)
	{
		MoveRight(inDuration, inSpeed);
		return RIGHT;
	}

	MoveLeft(inDuration, inSpeed);
	return LEFT;
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryMotorTwoWays::printAccessory()
{
	Serial.print(F("    MotorTwoWays : IDRight "));
	Serial.print(this->GetMovingPositionIdByIndex(0));
	if (this->GetMovingPositionSize() > 1)
	{
		Serial.print(F(" / IDLeft "));
		Serial.print(this->GetMovingPositionIdByIndex(1));
	}

	if (this->GetPort() != NULL)
	{
		Serial.print(F(" / Speed: "));
		Serial.print(this->GetPort()->GetSpeed());
	}
	Serial.print(F(" / Delay: "));
	Serial.print(this->GetDebounceDelay());
	Serial.print(F(" / "));
	if (this->GetPort() != NULL)
		this->GetPort()->printPort();
	Serial.println(F(" "));
}
#endif 

#endif
#endif
