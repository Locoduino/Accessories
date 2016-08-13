//-------------------------------------------------------------------
#ifndef __UAD_HPP__
#define __UAD_HPP__
//-------------------------------------------------------------------

#include "Accessories.h"

class Accessories
{
public:
	static void ReceiveEvent(unsigned long inID, ACCESSORIES_EVENT_TYPE lastEventType, int inData);
	static bool loop();

#ifdef DEBUG_MODE
	static void printEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData);
#endif
};

#endif
