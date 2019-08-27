/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "Accessories.hpp"
#include "ActionsStack.hpp"
#ifndef NO_EEPROM
#include "EEPROM.h"
#include "AccessoriesCircularBuffer.hpp"
#endif

bool Accessories::SerialStarted = false;
unsigned long Accessories::WaitEndDate = 0;
#ifndef NO_EEPROM
int Accessories::EEPROMStart = -1;
int Accessories::EEPROMSize = -1;
int Accessories::EEPROMRecordSize = 0;
unsigned long Accessories::EEPROMStartingDelay = 0;
AccessoriesCircularBuffer Accessories::circularBuffer;
#endif

void Accessories::begin(int inEEPROMStart, int inEEPROMSize)
{
	SerialStarted = true;

#ifndef NO_EEPROM
	Accessories::EEPROMStart = inEEPROMStart;
	Accessories::EEPROMSize = inEEPROMSize;
	Accessories::EEPROMStartingDelay = 0;
#endif

#ifdef ACCESSORIES_DEBUG_MODE
	// Just for let the time to the PIC to initialize internals...
	delay(500);

	Serial.println(F(""));
	Serial.println(F(ACCESSORIES_LIBRARY_VERSION));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F("(c) Locoduino 2016-2019"));
	Serial.println(F(""));

	Serial.println(F("*** Setup Accessories started."));

#ifndef NO_EEPROM
	if (EEPROMStart + EEPROMSize != -2 && (EEPROMSize == -1 || EEPROMStart == -1))
	{
		Serial.print(F("   Error : EEPROM will not be used : "));
		Serial.println(F("   EEPROMSize or EEPROMStart is not defined by begin."));
	}
#endif
#endif
}

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
	case ACCESSORIES_EVENT_SETSPEED:
		Serial.print(F("SET SPEED : "));
		Serial.println(inEventData);
		break;
	case ACCESSORIES_EVENT_SETDURATION:
		Serial.print(F("SET DURATION : "));
		Serial.println(inEventData);
		break;
		//case ACCESSORIES_EVENT_SETSTARTPOSITION:
	default:
		break;
	}
}
#endif

void Accessories::RaiseEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	ReceiveEvent(inId, inEvent, inData);
}

void Accessories::ReceiveEvent(unsigned long inId, ACCESSORIES_EVENT_TYPE inEventType, int inEventData)
{
#ifdef VISUALSTUDIO
	_eventLog(_T("Received"), inId, inEventType, inEventData);
#endif

	Accessory::ExecuteEvent(inId, inEventType, inEventData);
#ifndef NO_GROUP
	AccessoryGroup::EventAll(inId, inEventType, inEventData);
#endif
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void Accessories::printAccessories()
{
	Serial.println(F("********* Accessories List"));
	Accessory *pCurr = Accessory::GetFirstAccessory();

	while (pCurr != NULL)
	{
		pCurr->printAccessory();
		pCurr = pCurr->GetNextAccessory();
	}
	Serial.println(F("********* End of Accessories"));

#ifndef NO_GROUP
	Serial.println(F("********* Groups"));
	AccessoryGroup::printGroups();
	Serial.println(F("********* End of groups"));
#endif
}
#endif

static Accessory *pLoopAccessory = NULL;

bool Accessories::loop()
{
#ifndef NO_GROUP
	AccessoryGroup::loops();
#endif

	if (pLoopAccessory == NULL)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("*** Setup Accessories Finished."));

		Accessory *pCurr = Accessory::GetFirstAccessory();

		while (pCurr != NULL)
		{
			pCurr->CheckPort();
			pCurr = pCurr->GetNextAccessory();
		}

#ifndef NO_EXPANDER

		PortExpander::CheckAllExpanderPins();

#endif

#endif
		pLoopAccessory = Accessory::GetFirstAccessory();

#ifndef NO_EEPROM
		// Do not take account of the modified states during setup !
		EEPROMStartingDelay = 0;

#ifdef ACCESSORIES_DEBUG_MODE
		bool good = Accessories::EEPROMLoad();
		if (!good)
			Serial.println(F("EEPROM loading aborted !"));
#else
		Accessories::EEPROMLoad();
#endif
#endif
	}
	else
	{
		if (pLoopAccessory->GetNextAccessory() == NULL)
			pLoopAccessory = Accessory::GetFirstAccessory();
		else
			pLoopAccessory = pLoopAccessory->GetNextAccessory();
	}

	if (pLoopAccessory == NULL)
		return false;

	pLoopAccessory->loop();

	// Look for action stack pending...
	if (Accessory::IsActionPending())
		return true;

	unsigned char actionIndex = ActionsStack::Actions.GetActionToExecute();
	if (actionIndex != 255)
	{
		Action *act = ActionsStack::Actions[actionIndex];
		Accessory::ExecuteEvent(act->Id, act->Event, act->Data);
		ActionsStack::Actions.Delete(actionIndex);
	}

	// If nothing more to do, Save EEPROM if needed.

#ifndef NO_EEPROM
	if (EEPROMStartingDelay > 0)
		EEPROMSaveRaw();
#endif

	return false;
}

#ifndef NO_EEPROM

/*  EEPROM format

The EEPROM area starting from EEPROMStart, will be filled with the current version number (a byte), 
and the total number of accessories and groups. The fourth byte will be the checksum of the three
starting bytes...
If any of these four bytes are different from the actual values, the EEPROM will be considered as free.
Two bytes are then added to store the size of one complete record, in order to be able to configure the CircularBuffer.

Following these bytes, there is the CircularBuffer of accessories.
For each one the current state, the current position and the current speed will be saved.
After that, each group save its current item id.

						+-+
Version					|V|
Accessory number		|A|
Group number			|G|
header checksum			|C|
size byte 1				|s1]
size byte 2				|s2]
						+-+
Circular buffer			| |
						+-+
					End of file.

One record of the Circular buffer is :

						+-+
Acc 1 : State			|S|
Acc 1 : Position		|P|
Acc 1 : Speed			|s|
						+-+
Acc 2 : State			|S|
Acc 2 : Position		|P|
Acc 2 : Speed			|s|
						+-+					
Acc 3 : State			|S|
Acc 3 : Position		|P|
Acc 3 : Speed			|s|
...						+-+
Group 1 : current ID	|I|
Group 3 : current ID	|I|
Group 3 : current ID	|I|
						+-+
					End of record.
*/

// this version is used to know if the form have changed. During reloading, if the version is not equal to the 
// actual version EEPROM_VERSION, the file will be considered as empty !
#define EEPROM_VERSION	0

void Accessories::EEPROMSave()
{
	if (EEPROMStart == -1 || EEPROMSize == -1)
		return;

	if (EEPROMStartingDelay == 0)
		EEPROMStartingDelay = millis();
}

void Accessories::EEPROMSaveRaw()
{
	unsigned long mill = millis();
	if (mill - EEPROMStartingDelay < EEPROM_SAVE_DELAY)
		return;

	int pos = EEPROMStart;

	uint8_t accCount = Accessory::GetCount();
#ifndef NO_GROUP
	uint8_t grpCount = AccessoryGroup::GetCount();
#endif

	EEPROM.update(pos++, EEPROM_VERSION);
	EEPROM.update(pos++, accCount);
#ifndef NO_GROUP
	EEPROM.update(pos++, grpCount);
#endif

	EEPROM.update(pos++, EEPROMSize / 256);
	EEPROM.update(pos++, EEPROMSize % 256);

	if (EEPROMRecordSize == 0)
	{
		// Compute the size to save it for the first time.
		Accessory *pCurr = Accessory::GetFirstAccessory();

		while (pCurr != NULL)
		{
			EEPROMRecordSize = pCurr->EEPROMSave(EEPROMRecordSize, true);
			pCurr = pCurr->GetNextAccessory();
		}

#ifndef NO_GROUP
		EEPROMRecordSize = AccessoryGroup::EEPROMSaveAll(EEPROMRecordSize, true);
#endif

		circularBuffer.begin(pos+3, EEPROMRecordSize, (EEPROMSize - 10) / EEPROMRecordSize);
		circularBuffer.clear();
	}

	EEPROM.update(pos++, EEPROMRecordSize / 256);
	EEPROM.update(pos++, EEPROMRecordSize % 256);

	EEPROM.update(pos++, (uint8_t) (EEPROM_VERSION + accCount + 
#ifndef NO_GROUP
		grpCount +
#endif
		EEPROMSize + EEPROMRecordSize));

	pos = circularBuffer.startWrite();
	Accessory *pCurr = Accessory::GetFirstAccessory();

	while (pCurr != NULL)
	{
		pos = pCurr->EEPROMSave(pos);
		pCurr = pCurr->GetNextAccessory();
	}

#ifndef NO_GROUP
	pos = AccessoryGroup::EEPROMSaveAll(pos);
#endif
	EEPROMStartingDelay = 0;
}

bool Accessories::EEPROMLoad()
{
	if (EEPROMStart == -1 || EEPROMSize == -1)
		return false;

	int pos = EEPROMStart;
	uint8_t accCount = Accessory::GetCount();
#ifndef NO_GROUP
	uint8_t grpCount = AccessoryGroup::GetCount();
#endif

	if (EEPROM.read(pos++) != EEPROM_VERSION)
		return false;

	if (EEPROM.read(pos++) != accCount)
		return false;

#ifndef NO_GROUP
	if (EEPROM.read(pos++) != grpCount)
#endif
		if (EEPROM.read(pos++) != accCount)
			return false;

	byte b1, b2;
	b1 = EEPROM.read(pos++);
	b2 = EEPROM.read(pos++);
	if (EEPROMSize != b1 * 256 + b2)
		return false;

	b1 = EEPROM.read(pos++);
	b2 = EEPROM.read(pos++);
	EEPROMRecordSize = b1 * 256 + b2;

	if (EEPROM.read(pos++) != (uint8_t)(EEPROM_VERSION + accCount + 
#ifndef NO_GROUP
		grpCount +
#endif
		EEPROMSize + EEPROMRecordSize))
	{
		EEPROMRecordSize = 0;
		return false;
	}

	circularBuffer.begin(pos, EEPROMRecordSize, (EEPROMSize - 10) / EEPROMRecordSize);

	// Start circular buffer just after the header.
	pos = circularBuffer.getStartRead();
	Accessory *pCurr = Accessory::GetFirstAccessory();

	while (pCurr != NULL)
	{
		pos = pCurr->EEPROMLoad(pos);
		pCurr = pCurr->GetNextAccessory();
	}

#ifndef NO_GROUP
	pos = AccessoryGroup::EEPROMLoadAll(pos);
#endif
	EEPROMStartingDelay = 0;

	return true;
}
#endif
