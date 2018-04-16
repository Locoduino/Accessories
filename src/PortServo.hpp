//-------------------------------------------------------------------
#ifndef __PortServo_H__
#define __PortServo_H__
//-------------------------------------------------------------------

#ifndef NO_SERVO
#include "Servo.h"
#include "Accessories.h"

/**As the name of the class try to tell, this class s dedicated for a servo port.
It uses the library Servo installed with the Arduino IDE.
*/
class PortServo : public Port
{
	protected:  
		/** Command pin connected to the servo.*/
		int pin;    
		/** Instance of the class Servo.*/
		Servo servo;

	public:
		/**Default constructor.*/
		PortServo();
		
		/** Initialize the instance.
		@param inPin Command pin for the servo.
		*/
		void begin(int inPin);
		/** Initialize the starting position of the servo.
		@param inStartingPosition New position. The servo will physically move if necessary !
		*/
		void beginByAccessory(int inStartingPosition);

		/**Checks if this port is active. To be active, begin() must have been called.
		@return true if the power pin number is different than -1.
		*/
		inline bool IsActive() const { return this->pin > -1; }
		/**Gets the pin.
		@return Pin number in Arduino mode.
		*/
		inline int GetPin() const { return this->pin; }
		/**Gets the pin.
		@return Pin number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPin() const { return Arduino_to_GPIO_pin(this->pin); }
		
		/**Move the servo to the given position using a given duration.
		@param inDuration the movement should take the time of this delay.
		@param inEndPosition final position to reach.
		*/
		void MovePosition(unsigned long inDuration, int inEndPosition);
		/**Stop the servo : detach it.
		*/
		inline void MoveStop() { this->servo.detach(); }
		/**Gets the current position of the port.
		@return Current position of the port.
		*/
		int GetPosition();
#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/** Print one port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printPort();
#endif
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
