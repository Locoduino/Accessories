//-------------------------------------------------------------------
#ifndef __PortTwoPinsEnable_H__
#define __PortTwoPinsEnable_H__
//-------------------------------------------------------------------

#include "PortTwoPins.hpp"

/**This class describes a variant or the PortTwoPins class. There is the two originals pins from the base class,
but there is also a new pin to enable the full port.
*/
class PortTwoPinsEnable : public PortTwoPins
{
	protected:
		/**Enable pin of this port.
		ANALOG or ANALOG_INVERTED : Classic Arduino pin number
		DIGITAL or DIGITAL_INVERTED : GPIO_pin_t casted in an integer.
		*/
		int pinEnable;
		/**Enable pin type.
		In case of ANALOG or ANALOG_INVERTED this pin is used to give the PWM speed.
		*/
		PIN_TYPE pinTypeEnable;

	public:
		/**Default constructor.*/
		PortTwoPinsEnable();
		
		/** Initialize the instance.
		@param inPinA first pin to drive.
		@param inPinB second pin to drive.
		@param inType kind of both pins.
		@param inPinEnable enable pin.
		@param inTypeEnable kind of enable pin.
		*/
		void begin(int inPinA, int inPinB, PIN_TYPE inType, int inPinEnable, PIN_TYPE inTypeEnable);
		
		/**Gets the enable pin.
		@return pin number in Arduino mode.
		*/
		inline int GetPinEnable() const { if (this->GetPinType() < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinEnable); return this->pinEnable; }
		/**Gets the enable pin.
		@return Pin A number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPinEnable() const { if (this->GetPinType() < ANALOG) return (GPIO_pin_t)this->pinEnable; return DP_INVALID; }

		/**Sets the current state of the port to PORT_LEFT.
		@param inDuration the pins will be activated to left for the given delay.
		*/
		void MoveLeftDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_RIGHT.
		@param inDuration the pins will be activated to right for the given delay.
		*/
		void MoveRightDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_STOP.	*/
		void MoveStop();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/** Print this port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printPort();
#endif

private:
		void Move(int inValA, int inValB, int inValEnable);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
