//-------------------------------------------------------------------
#ifndef __PortTwoPins_H__
#define __PortTwoPins_H__
//-------------------------------------------------------------------

#include "Port.hpp"

/** This class describes a port for an accessory driven by two wires, two pins.
Some turnouts or motors can be driven by this port.*/
class PortTwoPins : public Port
{
	protected:
		/**First pin of this port.
		ANALOG or ANALOG_INVERTED : Classic Arduino pin number
		DIGITAL or DIGITAL_INVERTED : GPIO_pin_t casted in an integer.
		*/
		int pinA;
		/**Second pin of this port.
		ANALOG or ANALOG_INVERTED : Classic Arduino pin number
		DIGITAL or DIGITAL_INVERTED : GPIO_pin_t casted in an integer.
		*/
		int pinB;

	public:
		/**Default constructor.*/
		PortTwoPins();
		
		/** Initialize the instance.
		@param inPinA first pin to drive.
		@param inPinB second pin to drive.
		@param inType kind of both pins.
		*/
		void begin(int inPinA, int inPinB, PIN_TYPE inType);
		
		/**Checks if this port is active. To be active, begin() must have been called.
		@return true if the first pin number (pinA) is different than -1.
		*/
		inline bool IsActive() const { return this->pinA > -1; }
		/**Gets the first Arduino pin.
		@return Pin A number in Arduino mode.
		*/
		inline int GetPinA() const { if (this->GetPinType() < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinA); return this->pinA; }
		/**Gets the first Arduino pin.
		@return Pin A number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPinA() const { if (this->GetPinType() < ANALOG) return (GPIO_pin_t)this->pinA; return DP_INVALID; }
		/**Gets the second Arduino pin.
		@return Pin B number in Arduino mode.
		*/
		inline int GetPinB() const { if (this->GetPinType() < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pinB); return this->pinB; }
		/**Gets the second Arduino pin.
		@return Pin B number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPinB() const { if (this->GetPinType() < ANALOG) return (GPIO_pin_t)this->pinB; return DP_INVALID; }

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
		void Move(int inValA, int inValB);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
