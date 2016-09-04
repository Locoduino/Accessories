//-------------------------------------------------------------------
#ifndef __Accessories_HPP__
#define __Accessories_HPP__
//-------------------------------------------------------------------

#include "Accessories.h"

class Accessories
{
public:
	static bool SerialStarted;

public:
	static void begin();
	static void RaiseEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
	static void ReceiveEvent(unsigned long inID, ACCESSORIES_EVENT_TYPE lastEventType, int inData);
	static bool loop();

#ifdef ACCESSORIES_DEBUG_MODE
	static void printEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData);
#endif
};

#endif
