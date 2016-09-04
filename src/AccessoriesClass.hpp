//-------------------------------------------------------------------
#ifndef __accessories_H__
#define __accessories_H__
//-------------------------------------------------------------------

#include "Accessories.h"

//-------------------------------------------------------------------

// Describes static data only intialized once at the beginning of execution.

class AccessoriesClass
{
	public:
		static AccessoriesClass AccessoriesInstance;
	   
	private:
		static Accessory *pFirstAccessory;

	protected:
	
	public:
		AccessoriesClass();
		
		Accessory *GetById(unsigned long inId) const;
		void begin();

		bool loop();

		bool CanMove(unsigned long inId);

		bool Toggle(unsigned long inId);
		bool MovePosition(unsigned long inId);
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
		bool IsActionPending();

		static void Add(Accessory *inpAccessory);
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
