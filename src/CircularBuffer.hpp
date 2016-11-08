//-------------------------------------------------------------------
#ifndef __Accessories_CircularBuffer_H__
#define __Accessories_CircularBuffer_H__
//-------------------------------------------------------------------

class AccessoriesCircularBuffer
{
public:
	int StartListPos;
	byte ItemSize;
	byte ReplicaNumber;

	AccessoriesCircularBuffer() { this->ItemSize = 0; this->ReplicaNumber = 0; this->StartListPos = 0; }

	int begin(int inStartListPos, byte inItemSize, byte inReplicaNumber) 
	{ 
		this->StartListPos = inStartListPos;  
		this->ItemSize = inItemSize; 
		this->ReplicaNumber = inReplicaNumber; 

		return (this->ItemSize + 1) * this->ReplicaNumber;
	}

	int getStartRead();
	int startWrite();

	void clear() const;

#ifdef ACCESSORIES_DEBUG_MODE
	void printStatus();
#endif

private:
	byte FindEnd();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
