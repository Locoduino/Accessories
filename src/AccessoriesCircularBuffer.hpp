//-------------------------------------------------------------------
#ifndef __Accessories_CircularBuffer_H__
#define __Accessories_CircularBuffer_H__
//-------------------------------------------------------------------

#ifndef NO_EEPROM
/**
* This class implements the Atmel solution to avoid writing in EEPROM always in the same area to improve lifetime
* of this memory : a Circular Buffer. See http://www.atmel.com/images/doc2526.pdf .
* Part of the PDF file :
*
<blockquote>By having a circular buffer (O-buffer) in EEPROM it is possible to increase the number
of times that a parameter can be stored in EEPROM. If the buffer has two levels the
number of times that the parameter can be stored is twice the endurance of one single
EEPROM cell: 200 k erase/writes. By using this approach it is possible to increase the
number of times a parameter can be stored by increasing the O-buffer size. In other
word, the idea is to distribute the storage of the parameter over several EEPROM
locations to achieve increased parameter storage endurance. When using the Obuffer
approach the parameter storage endurance equal the number of memory locations
used in the O-buffer times the endurance of a single EEPROM location.</blockquote>

A circular buffer is a way to reduce the use of each EEPROM cell, and improve life time of the full EEPROM memory.
An index of bytes is stored at the beginning of the area. Each index represents a data area.

\verbatim
|0|1|2|3|4||---0---|---1---|---2---|---3---|---4---|
\endverbatim
*/
class AccessoriesCircularBuffer
{
private:
	int StartListPos;
	byte ItemSize;
	byte ReplicaNumber;

public:
	/**	Basic constructor.*/
	AccessoriesCircularBuffer() { this->ItemSize = 0; this->ReplicaNumber = 0; this->StartListPos = 0; }

	/** Initialize the instance.
	@param inStartListPos	EEPROM address of the beginning of the memory used for the storage
	@param inItemSize Size in bytes of one item to save.
	@param inReplicaNumber Number of copies of the element can be reserved for the buffer. 2 is a minimum, 255 a maximum.
	@return Number of EEPROM bytes reserved for the circular buffer.
	*/
	int begin(int inStartListPos, byte inItemSize, byte inReplicaNumber)
	{ 
		this->StartListPos = inStartListPos;  
		this->ItemSize = inItemSize; 
		this->ReplicaNumber = inReplicaNumber; 

		return (this->ItemSize + 1) * this->ReplicaNumber;
	}

	/** If data cannot be loaded with a single call to a 'read', this function can be called to know where to read the current good element.
	@return EEPROM address to start reading data.
	*/
	int getStartRead();
	/** If data cannot be stored with a single call to a 'write', this function can be called to prepare the saving of the next good element.
	@return EEPROM address to start writing data. Be careful, do not store more bytes than the inElementSize given during begin() call !
	*/
	int startWrite();

	/** Fill with 0s the area reserved for the buffer. Use with care !*/
	void clear() const;

#ifdef ACCESSORIES_DEBUG_MODE
	/** In debug mode, with EEPROMEXTENT_DEBUG_MODE uncommented, this function will show on console the actual structure of the buffer.*/
	void printStatus();
#endif

private:
	byte FindEnd();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
