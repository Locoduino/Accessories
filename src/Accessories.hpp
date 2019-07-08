//-------------------------------------------------------------------
#ifndef __Accessories_HPP__
#define __Accessories_HPP__
//-------------------------------------------------------------------

#include "Accessories.h"
#include "AccessoriesCircularBuffer.hpp"

/**Delay before saving. The saving on EEPROM is only done when no other saving has been done since at least this delay.*/
#define EEPROM_SAVE_DELAY	1000

/**
This is the main class of the library. All data and functions are static.
There is no way to instantiate this class.
*/
class Accessories
{
private:
	static bool SerialStarted;
	static unsigned long WaitEndDate;
#ifndef NO_EEPROM
	static int EEPROMStart;
	static int EEPROMSize;
	static int EEPROMRecordSize;
	static unsigned long EEPROMStartingDelay;
	static AccessoriesCircularBuffer circularBuffer;
#endif

public:
	/** Initialize the instance.
	@param inEEPROMStart	Defines the starting byte address to save accessories status.
	If the size is -1, there will be no EEPROM saving. Default is -1.
	@param inEEPROMSize		Defines the total size of EEPROM memory available to save accessories status.
	If the size is -1, there will be no EEPROM saving. Default is -1.
	*/
	static void begin(int inEEPROMStart = -1, int inEEPROMSize = -1);
	/** Raise a new event.
	@param inId Id of a MovePosition of an accessory or an accessory item.
	@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
	@param inData Associated data to the event type. Default is 0.
	*/
	static void RaiseEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
	/** Receive an event from external source.
	@param inID Id of the an accessory or an accessory item.
	@param lastEventType Type of the new event.
	@param inData Associated data to the event type.
	*/
	static void ReceiveEvent(unsigned long inID, ACCESSORIES_EVENT_TYPE lastEventType, int inData);
	/** Main loop function.
	This function will call all necessary internal loop functions for each accessory activated...
	@return True if something rest to execute.
	*/
	static bool loop();
#ifndef NO_EEPROM
	/** Save all necessary data to EEPROM.*/
	static void EEPROMSave();
#endif

#ifdef ACCESSORIES_DEBUG_MODE
	/** Print on the console the given event in plain English.
	@remark Only available if ACCESSORIES_DEBUG_MODE is defined.
	*/
	static void printEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData);
#endif

private:
#ifndef NO_EEPROM
	static void EEPROMSaveRaw();
	static bool EEPROMLoad();
#endif

public:
#ifdef ACCESSORIES_PRINT_ACCESSORIES
	/** This define is empty if ACCESSORIES_PRINT_ACCESSORIES is not defined. */
	#define PRINT_ACCESSORIES	Accessories::printAccessories();
	/** Print the full list of declared accessories.
	The list is printed on the console in a hierarchical way, with details like accessories and ports. There is also all data (pins, events)
	associated with each item.
	@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
	*/
	static void printAccessories();
#else
	#define PRINT_ACCESSORIES
#endif
};

#endif
