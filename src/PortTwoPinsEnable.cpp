/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port with 2 pins and an Enable pin>
*************************************************************/

#include "Accessories.h"

PortTwoPinsEnable::PortTwoPinsEnable() : PortTwoPins()
{
}

void PortTwoPinsEnable::begin(int inPinA, int inPinB, PIN_TYPE inType, int inPinEnable, PIN_TYPE inTypeEnable)
{
	this->PortTwoPins::begin(inPinA, inPinB, inType);

	this->pinTypeEnable = inTypeEnable;
	this->pinEnable = this->beginPin(inPinEnable, inTypeEnable);

	CHECKPIN(this->pinEnable, inType, "PortTwoPinsEnable::begin");
}

void PortTwoPinsEnable::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F(" PortTwoPinsEnable MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	this->Move(LOW, HIGH, HIGH);

	if (inDuration != 0)
	{
		delay(inDuration);

		this->Move(LOW, LOW, LOW);
	}

	this->SetPortState(PORT_LEFT);
}

void PortTwoPinsEnable::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F(" PortTwoPinsEnable MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif

	this->Move(HIGH, LOW, HIGH);

	if (inDuration != 0)
	{
		delay(inDuration);

		this->Move(LOW, LOW, LOW);
	}

	this->SetPortState(PORT_RIGHT);
}

void PortTwoPinsEnable::MoveStop()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" / "));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.println(F(" PortTwoPinsEnable MoveStop() "));
#endif

	this->Move(LOW, LOW, LOW);

	this->SetPortState(PORT_STOP);
}

void PortTwoPinsEnable::Move(int inValA, int inValB, int inValEnable)
{
	MovePin(this->pinA, inValA);
	MovePin(this->pinB, inValB);

	MovePin(this->pinEnable, inValEnable, this->pinTypeEnable);
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void PortTwoPinsEnable::printPort()
{
	Serial.print(F("[PortTwoPinsEnable pinA:"));
	Port::printPortPin(this->pinA, this->GetPinType());
	Serial.print(F(" pinB:"));
	Port::printPortPin(this->pinB, this->GetPinType());
	Serial.print(F(" enable:"));
	Port::printPortPin(this->pinEnable, this->pinTypeEnable);
	Serial.print(F("]"));
}
#endif
