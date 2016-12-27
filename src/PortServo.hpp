//-------------------------------------------------------------------
#ifndef __PortServo_H__
#define __PortServo_H__
//-------------------------------------------------------------------

#ifndef NO_SERVO
#include "Servo.h"
#include "Accessories.h"

class PortServo : public Port
{
	protected:      
		int pin;    
		Servo servo;

	public:
		PortServo();
		
		void begin(int inPin);
		void beginByAccessory(int inStartingPosition);

		inline bool IsActive() const { return this->pin > -1; }
		inline int GetPin() const { return this->pin; }
		inline GPIO_pin_t GetPin2() const { return Arduino_to_GPIO_pin(this->pin); }
		
		void MovePosition(unsigned long inDuration, int inEndPosition);
		int GetPosition();
#ifdef ACCESSORIES_PRINT_ACCESSORIES
		void printPort();
#endif
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
