/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Accessories.hpp"

// Link to Commanders library.

#ifdef DEBUG_MODE
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
	case ACCESSORIES_EVENT_CONFIG:
		Serial.print(F("CONFIG : "));
		Serial.print(ACCESSORIESCONFIGADDRESS(inEventData), DEC);
		Serial.print(F(" / "));
		Serial.println(ACCESSORIESCONFIGVALUE(inEventData), DEC);
		break;
	}
}
#endif

void Accessories::ReceiveEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData)
{
#ifdef VISUALSTUDIO
	ArduiEmulator::ArduinoForm::_eventLog("Received", inId, inEventType, inEventData);
#endif

	AccessoriesClass::AccessoriesInstance.Event(inId, inEventType, inEventData);
	AccessoryGroup::EventAll(inId, inEventType, inEventData);
}

bool Accessories::loop()
{
	bool ret = AccessoryGroup::loops();

	if (ret)
		return true;

	return AccessoriesClass::AccessoriesInstance.loop();
}
