//-------------------------------------------------------------------
#ifndef __driverPortServo_H__
#define __driverPortServo_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

#ifndef NO_SERVO
class DriverPortServo: public DriverPort
{
	protected:

	public:
		DriverPortServo(byte inId);
		
		virtual void begin();
		virtual void beginByAccessory(int inStartingPosition);

		virtual void MovePosition(unsigned long inDuration, int inEndPosition);
		virtual int GetPosition();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
