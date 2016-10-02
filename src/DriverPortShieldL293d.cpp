/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Driver port for L293n>
*************************************************************/

#if !defined(__AVR_ATmega32U4__)

#include "Accessories.h"

#ifndef NO_L293D

DriverPortShieldL293d::DriverPortShieldL293d(uint8_t inId, unsigned char inOutPort, uint8_t inSpeed, uint8_t inFreq) : DriverPort(MOTOR_LIGHT, inId)
{
	this->pmotor = new AF_DCMotor(inOutPort, inFreq);
	this->pmotor->setSpeed(inSpeed);
}

void DriverPortShieldL293d::begin(uint8_t inFreq)
{
	this->pmotor->pwmfreq = inFreq;

	this->pmotor->setSpeed(this->GetSpeed());
 
	this->pmotor->run(RELEASE);
}

int DriverPortShieldL293d::SetSpeed(uint8_t inSpeed)
{
	int oldspeed = DriverPort::SetSpeed(inSpeed);
	this->pmotor->setSpeed(inSpeed);
	return oldspeed;
}

void DriverPortShieldL293d::MoveLeftDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" DriverPortShieldL293d MoveLeftDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->pmotor->run(FORWARD);
	if (inDuration != 0)
	{
		delay(inDuration);
		this->pmotor->run(RELEASE);
	}

	this->state = PORT_LEFT;
}

void DriverPortShieldL293d::MoveRightDir(unsigned long inDuration)
{
#ifdef ACCESSORIES_DEBUG_MODE
	//Serial.print(this->pmotor->motornum);
	Serial.print(F(" DriverPortShieldL293d MoveRightDir() "));
	if (inDuration != 0)
	{
		Serial.print(F("for "));
		Serial.print(inDuration);
		Serial.println(F("ms"));
	}
	else
		Serial.println("");
#endif
	this->pmotor->run(BACKWARD);
	if (inDuration != 0)
	{
		delay(inDuration);
		this->pmotor->run(RELEASE);
	}

	this->state = PORT_RIGHT;
}

void DriverPortShieldL293d::MoveStop()
{
	this->pmotor->run(RELEASE);
	this->state = PORT_STOP;
}
#endif
#endif
