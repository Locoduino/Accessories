/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for the Arduino itself, as a driver>
*************************************************************/

#include "Accessories.h"

#ifndef NO_ARDUINODRIVER

DriverArduino::DriverArduino() : Driver()
{
}

DriverPortArduino *DriverArduino::AddPortMotor(int inPin, PORT_TYPE inType)
{
	DriverPortArduino *port = new DriverPortArduino((uint8_t) inPin);
	this->AddPort(port);
	port->begin(inPin, inType);

	return port;
}

#if !defined(NO_SERVO)
DriverPortServo *DriverArduino::AddPortServo(int inPin, PORT_TYPE inType)
{
	DriverPortServoBase *port = new DriverPortServoBase((uint8_t)inPin);
	this->AddPort(port);
	port->begin(inPin, inType);

	return port;
}
#endif


#endif
