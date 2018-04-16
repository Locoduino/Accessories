/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for a LMD18200>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR_LIGHT

PortSpeedDirBrake::PortSpeedDirBrake()
{
	this->pinPWM = -1;
}

void PortSpeedDirBrake::begin(int inPinPWM, int inPinDir, int inPinBrake, PIN_TYPE inPWMType, PIN_TYPE inDigitalType)
{
#ifdef ACCESSORIES_DEBUG_MODE
	if ((int)inPWMType < ANALOG)
		Serial.print(F(" PortSpeedDirBrake begin() : inPWMType must be ANALOG or ANALOG_INVERTED !"));
	if ((int)inDigitalType >= ANALOG)
		Serial.print(F(" PortSpeedDirBrake begin() : inDigitalType must be DIGITAL or DIGITAL_INVERTED !"));
#endif

	this->pinPWM = inPinPWM;
	this->pinDir = Arduino_to_GPIO_pin(inPinDir);
	this->pinBrake = Arduino_to_GPIO_pin(inPinBrake);

	this->SetPinType(inPWMType);
	this->digitalType = inDigitalType;

	this->beginPin(this->pinPWM);
	this->beginPin(this->pinDir, this->digitalType);
	this->beginPin(this->pinBrake, this->digitalType);
}

int PortSpeedDirBrake::MapDigitalValue(int inValue) const
{
	if (this->GetPinType() == DIGITAL_INVERTED)
	{
		if (inValue == LOW)
			return HIGH;
		return LOW;
	}

	return inValue;
}

void PortSpeedDirBrake::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortSpeedDirBrake MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->SetPortState(PORT_LEFT);

	this->MovePin(this->pinPWM, HIGH);
	this->MovePin(this->pinDir, LOW, this->digitalType);
	this->MovePin(this->pinBrake, LOW, this->digitalType);
}

void PortSpeedDirBrake::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" PortSpeedDirBrake MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->SetPortState(PORT_RIGHT);
	this->MovePin(this->pinPWM, HIGH);
	this->MovePin(this->pinDir, HIGH, this->digitalType);
	this->MovePin(this->pinBrake, LOW, this->digitalType);
}

void PortSpeedDirBrake::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F(" PortSpeedDirBrake MoveStop()"));
#endif
	this->SetPortState(PORT_STOP);
	this->MovePin(this->pinBrake, HIGH, this->digitalType);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortSpeedDirBrake::printPort()
{
	Serial.print(F("[PortSpeedDirBrake pinPWM:"));
	Port::printPortPin(this->pinPWM, this->GetPinType());
	Serial.print(F(" dir:"));
	Port::printPortPin(this->pinDir, this->digitalType);
	Serial.print(F(" brake:"));
	Port::printPortPin(this->pinBrake, this->digitalType);
	Serial.print(F("]"));
}
#endif

#endif
