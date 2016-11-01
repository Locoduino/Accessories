//-------------------------------------------------------------------
#ifndef __Accessories_HPP__
#define __Accessories_HPP__
//-------------------------------------------------------------------

#include "Accessories.h"
//#include "AccessoriesClass.hpp"

#define EEPROM_SAVE_DELAY	1000

class Accessories
{
public:
	static bool SerialStarted;
#ifndef NO_EEPROM
	static int EEPROMStart;
	static unsigned long EEPROMStartingDelay;
#endif

public:
	static void begin(int inEEPROMStart = -1);
	static void RaiseEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
	static void ReceiveEvent(unsigned long inID, ACCESSORIES_EVENT_TYPE lastEventType, int inData);
	static bool loop();
#ifndef NO_EEPROM
	static void EEPROMSave();
#endif

#ifdef ACCESSORIES_DEBUG_MODE
	static void printEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData);
#endif

private:
	static Accessory *pFirstAccessory;

	static Accessory *GetById(unsigned long inId);

	static bool CanMove(unsigned long inId);

	static bool Toggle(unsigned long inId);
	static bool MovePosition(unsigned long inId);
	static void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
	static bool IsActionPending();
	static uint8_t GetCount();
#ifndef NO_EEPROM
	static void EEPROMSaveRaw();
	static bool EEPROMLoad();
#endif

public:
	static void Add(Accessory *inpAccessory);
};

#endif
