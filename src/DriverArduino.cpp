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
	DriverPortArduino *port = new DriverPortArduino((byte) inPin);
	this->AddPort(port);
	port->begin(inPin, inType);

	return port;
}

DriverPortServo *DriverArduino::AddPortServo(int inPin, PORT_TYPE inType)
{
#if !defined(NO_SERVO)
	DriverPortServoArduino *port = new DriverPortServoArduino((byte)inPin);
	this->AddPort(port);
	port->begin(inPin, inType);

	return port;
#endif
}


#endif
