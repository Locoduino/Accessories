/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a L293D driver>
*************************************************************/

#if !defined(__AVR_ATmega32U4__)
#include "Accessories.h"

#ifndef NO_L293D

DriverL293d::DriverL293d()
{
#if !defined(NO_MOTOR_LIGHT)
	this->AddPort(new DriverPortL293d(L293D_PORT_M1, 1, 200, MOTOR12_1KHZ));
	this->AddPort(new DriverPortL293d(L293D_PORT_M2, 2, 200, MOTOR12_1KHZ));
	this->AddPort(new DriverPortL293d(L293D_PORT_M3, 3, 200, MOTOR34_1KHZ));
	this->AddPort(new DriverPortL293d(L293D_PORT_M4, 4, 200, MOTOR34_1KHZ));
#endif

#if !defined(NO_SERVO)
	this->AddPort(new DriverPortServoArduino(L293D_PORT_SERVO1));
	this->AddPort(new DriverPortServoArduino(L293D_PORT_SERVO2));
#endif

/*
#if !defined(NO_STEPPER)
	this->AddPort(new DriverPortStepper2(L293D_PORT_STEPPER12));
	this->AddPort(new DriverPortStepper2(L293D_PORT_STEPPER34));
	this->AddPort(new DriverPortStepper4(L293D_PORT_STEPPER1234));
#endif
*/
}

void DriverL293d::begin()
{
	Driver::begin();

#if !defined(NO_SERVO)
	((DriverPortServoArduino *) (this->GetPort(SERVO, L293D_PORT_SERVO1)))->begin(SERVO1_PIN, ANALOG);
	((DriverPortServoArduino *) (this->GetPort(SERVO, L293D_PORT_SERVO2)))->begin(SERVO2_PIN, ANALOG);
#endif
}

DriverPortL293d * DriverL293d::beginPortMotor(byte inPort, uint8_t inFreq)
{
#if !defined(NO_MOTOR_LIGHT)
	DriverPortL293d *pPort = (DriverPortL293d *) this->GetPort(MOTOR_LIGHT, inPort);
	pPort->begin(inFreq);
	return pPort;
#endif
}

DriverPortServoArduino * DriverL293d::beginPortServo(byte inPort)
{
#if !defined(NO_SERVO)
	DriverPortServoArduino *pPort = (DriverPortServoArduino *) this->GetPort(SERVO, inPort);
#ifdef DEBUG_MODE
	if (pPort->GetPin() == -1)
	{
		Serial.print(F("Port Servo Arduino "));
		Serial.print(inPort);
		Serial.print(F(" error in DriverL293d::beginPortServo : driver.begin() not called."));
	}
#endif

	return pPort;
#endif
}

/*
DriverPortStepper * DriverL293d::beginPortStepper(byte inPort)
{
#if !defined(NO_STEPPER)
	DriverPortStepper *pPort = (DriverPortStepper *) this->GetPort(STEPPER, inPort);
	pPort->begin(0);
	return pPort;
#endif
}
*/

#endif
#endif