/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port with 2 pins>
*************************************************************/

#include "Accessories.h"

PortTwoPins::PortTwoPins()
{
}

void PortTwoPins::begin(int inPinA, int inPinB, PIN_TYPE inType)
{
	this->SetPinType(inType);

	this->pinA = this->beginPin(inPinA);
	this->pinB = this->beginPin(inPinB);
}

void PortTwoPins::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F(" PortTwoPins MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	this->Move(HIGH, LOW);

	if (inDuration != 0)
	{
		delay(inDuration);

		this->Move(LOW, LOW);
	}

	this->SetPortState(PORT_LEFT);
}

void PortTwoPins::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F(" PortTwoPins MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	this->Move(LOW, HIGH);

	if (inDuration != 0)
	{
		delay(inDuration);

		this->Move(LOW, LOW);
	}

	this->SetPortState(PORT_RIGHT);
}

void PortTwoPins::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.println(F(" PortTwoPins MoveStop() "));
#endif

	this->Move(LOW, LOW);

	this->SetPortState(PORT_STOP);
}

void PortTwoPins::Move(int inValA, int inValB)
{
	this->MovePin(this->pinA, inValA);
	this->MovePin(this->pinB, inValB);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortTwoPins::printPort()
{
	Serial.print(F("[PortTwoPins pinA:"));
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" pinB:"));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F("]"));
}
#endif

