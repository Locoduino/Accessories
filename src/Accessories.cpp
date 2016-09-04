/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Accessories.hpp"

bool Accessories::SerialStarted = false;

// Link to Commanders library.

#ifdef ACCESSORIES_DEBUG_MODE
void Accessories::printEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData)
{
	Serial.print(F("Commander event : Address : "));
	Serial.print(inId, DEC);
	Serial.print(F(" / "));
	switch (inEventType)
	{
	case ACCESSORIES_EVENT_NONE:			Serial.println(F("NONE"));		break;
	case ACCESSORIES_EVENT_TOGGLE:		Serial.println(F("TOGGLE"));	break;
	case ACCESSORIES_EVENT_MOVE:
		Serial.print(F("MOVE "));
		switch ((ACCESSORIES_MOVE_TYPE)inEventData)
		{
		case ACCESSORIES_MOVE_MORE:		Serial.println(F("MORE"));		break;
		case ACCESSORIES_MOVE_LESS:		Serial.println(F("LESS"));		break;
		case ACCESSORIES_MOVE_STOP:		Serial.println(F("STOP"));		break;
		case ACCESSORIES_MOVE_LEFT:		Serial.println(F("LEFT"));		break;
		case ACCESSORIES_MOVE_RIGHT:		Serial.println(F("RIGHT"));		break;
		case ACCESSORIES_MOVE_CENTER:	Serial.println(F("CENTER"));	break;
		case ACCESSORIES_MOVE_TOP:		Serial.println(F("TOP"));		break;
		case ACCESSORIES_MOVE_BOTTOM:	Serial.println(F("BOTTOM"));	break;
		case ACCESSORIES_MOVE_STRAIGHT:	Serial.println(F("STRAIGHT"));	break;
		case ACCESSORIES_MOVE_DIVERGE:	Serial.println(F("DIVERGE"));	break;
		case ACCESSORIES_MOVE_ON:		Serial.println(F("ON"));		break;
		case ACCESSORIES_MOVE_OFF:		Serial.println(F("OFF"));		break;
		}
		break;
	case ACCESSORIES_EVENT_MOVEPOSITION:
		Serial.print(F("MOVEPOSITION : "));
		Serial.println(inEventData, DEC);
		break;
	case ACCESSORIES_EVENT_MOVEPOSITIONID:
		Serial.println(F("MOVEPOSITIONID "));
		break;
	case ACCESSORIES_EVENT_MOVEPOSITIONINDEX:
		Serial.print(F("MOVEPOSITIONINDEX : "));
		Serial.println(inEventData, DEC);
		break;
	case ACCESSORIES_EVENT_CONFIG:
		Serial.print(F("CONFIG : "));
		Serial.print(ACCESSORIESCONFIGADDRESS(inEventData), DEC);
		Serial.print(F(" / "));
		Serial.println(ACCESSORIESCONFIGVALUE(inEventData), DEC);
		break;
	//case ACCESSORIES_EVENT_SETSPEED:
	//case ACCESSORIES_EVENT_SETSTARTPOSITION:
	default:
		break;
	}
}
#endif

void Accessories::begin()
{
	SerialStarted = true;
#ifdef ACCESSORIES_DEBUG_MODE
//	Serial.begin(115200);
	// Just for let the time to the PIC to initialize internals...
	delay(500);

	Serial.println(F(""));
	Serial.println(F("Accessories V0.20"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F("(c) Locoduino 2016"));
	Serial.println(F(""));

	Serial.println(F("*** Setup Accessories started."));
#endif
}

void Accessories::RaiseEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	ReceiveEvent(inId, inEvent, inData);
}

void Accessories::ReceiveEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData)
{
#ifdef VISUALSTUDIO
	ArduiEmulator::ArduinoForm::_eventLog("Received", inId, inEventType, inEventData);
#endif

	AccessoriesClass::AccessoriesInstance.Event(inId, inEventType, inEventData);
#ifndef NO_GROUP
	AccessoryGroup::EventAll(inId, inEventType, inEventData);
#endif
}

bool Accessories::loop()
{
#ifndef NO_GROUP
	bool ret = AccessoryGroup::loops();

	if (ret)
		return true;
#endif

	return AccessoriesClass::AccessoriesInstance.loop();
}
