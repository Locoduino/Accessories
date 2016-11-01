//-------------------------------------------------------------------
#ifndef __PortTwoPins_H__
#define __PortTwoPins_H__
//-------------------------------------------------------------------

#include "Port.hpp"

class PortTwoPins : public Port
{
	protected:
		int pinA, pinB;

	public:
		PortTwoPins();
		
		void begin(int inPinA, int inPinB, PIN_TYPE inType);
		
		inline bool IsActive() const { return this->pinA > -1; }
		inline int GetPinA() const { if (pinType < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinA); return this->pinA; }
		inline GPIO_pin_t GetDIOPinA() const { if (pinType < ANALOG) return (GPIO_pin_t)this->pinA; return DP_INVALID; }
		inline int GetPinB() const { if (pinType < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinB); return this->pinB; }
		inline GPIO_pin_t GetDIOPinB() const { if (pinType < ANALOG) return (GPIO_pin_t)this->pinB; return DP_INVALID; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();

	private:
		void Move(int inValA, int inValB);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
