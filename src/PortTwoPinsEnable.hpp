//-------------------------------------------------------------------
#ifndef __PortTwoPinsEnable_H__
#define __PortTwoPinsEnable_H__
//-------------------------------------------------------------------

#include "PortTwoPins.hpp"

class PortTwoPinsEnable : public PortTwoPins
{
	protected:
		int pinEnable;
		PIN_TYPE pinTypeEnable;

	public:
		PortTwoPinsEnable();
		
		void begin(int inPinA, int inPinB, PIN_TYPE inType, int inPinEnable, PIN_TYPE inTypeEnable);
		
		inline int GetPinEnable() const { if (pinType < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinEnable); return this->pinEnable; }
		inline GPIO_pin_t GetDIOPinEnable() const { if (pinType < ANALOG) return (GPIO_pin_t)this->pinEnable; return DP_INVALID; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		void printPort();
#endif

private:
		void Move(int inValA, int inValB, int inValEnable);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
