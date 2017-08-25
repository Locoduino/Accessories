/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a circular buffer of data stored in EEPROM>
*************************************************************/

// Copied from EEPROMExtent library

#include "Accessories.h"						  
#ifndef NO_EEPROM
#include "AccessoriesCircularBuffer.hpp"
#include <EEPROM.h>

/*
A circular buffer is a way to reduce the use of each EEPROM cell,
and improve life time of the full EEPROM memory.
An index of bytes is stored at the beginning of the area.
Each index represents a data area.

|0|1|2|3|4||---0---|---1---|---2---|---3---|---4---|

*/

byte AccessoriesCircularBuffer::FindEnd()
{
	/* 
	prev is the previous value from the item in the list
	|..|..|prev|i|..|..|

	we begin with the first item of the list. In this case, the previous is the last one:
	|i|..|..|..|..|prev|
	*/

	int prevpos = this->StartListPos + this->ReplicaNumber - 1;
	byte prev = EEPROM.read(prevpos);

	for (int i = 0; i < this->ReplicaNumber; i++)
	{
		int pos = this->StartListPos + i;

		// Checks it the current value is really the previous value + 1 : 
		// 4 must be 3+1, 0 must be 255 + 1 !
		if (prev + 1 != EEPROM.read(pos))
			return prevpos - this->StartListPos;

		prev = EEPROM.read(pos);
		prevpos = pos;
	}

	// Should never reach this code !

	return 255;
}

void AccessoriesCircularBuffer::clear() const
{
	for (int i = this->StartListPos; i < this->StartListPos + (this->ItemSize + 1) * this->ReplicaNumber; i++)
		EEPROM.write(i, 0);
}

int AccessoriesCircularBuffer::getStartRead()
{
	byte place = FindEnd();
	return this->StartListPos + this->ReplicaNumber + (this->ItemSize * place);
}

int AccessoriesCircularBuffer::startWrite()
{
	byte place = FindEnd();
	byte itemNb = EEPROM.read(this->StartListPos + place);

	place++;
	if (place >= this->ReplicaNumber)
		place = 0;

	EEPROM.update(this->StartListPos + place, ++itemNb);

	return this->StartListPos + this->ReplicaNumber + (this->ItemSize * place);
}

#ifdef ACCESSORIES_DEBUG_MODE
void AccessoriesCircularBuffer::printStatus()
{
	Serial.print(F("CB Status : "));
	for (int i = 0; i < this->ReplicaNumber; i++)
	{
		Serial.print(F("|"));
		Serial.print(EEPROM.read(this->StartListPos + i));
	}
	Serial.println(F("|"));
}
#endif
#endif