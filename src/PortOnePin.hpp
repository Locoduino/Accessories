//-------------------------------------------------------------------
#ifndef __PortOnePin_H__
#define __PortOnePin_H__
//-------------------------------------------------------------------

#include "Port.hpp"

class PortOnePin : public Port
{
	protected:
		int pin;	// ANALOG or ANALOG_INVERTED : Classic Arduino pin number
					// DIGITAL or DIGITAL_INVERED : GPIO_pin_t casted in an integer.

	public:
		PortOnePin();
							  
		void begin(int inPin, PIN_TYPE inType);
		
		int SetSpeed(int inSpeed);

		inline bool IsActive() const { return this->pin > -1; }
		inline int GetPin() const { if (this->pinType < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pin); return this->pin;  }
		inline GPIO_pin_t GetDIOPin() const { if (this->pinType >= ANALOG) return (GPIO_pin_t)this->pin; return DP_INVALID; }
		
		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop(); 

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		void printPort();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
