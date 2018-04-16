//-------------------------------------------------------------------
#ifndef __PortOnePin_H__
#define __PortOnePin_H__
//-------------------------------------------------------------------

#include "Port.hpp"

/** This class describes a port for a simple accessory driven by only one wire, one pin.
Lights and uncoupling track can be driven by this kind of port.*/
class PortOnePin : public Port
{
	protected:
		/**Main pin of this port.
		ANALOG or ANALOG_INVERTED : Classic Arduino pin number
		DIGITAL or DIGITAL_INVERTED : GPIO_pin_t casted in an integer.		
		*/
		int pin;

	public:
		/**Default constructor.*/
		PortOnePin();
							  
		/** Initialize the instance.
		@param inPin pin to drive.
		@param inType kind of pin.
		*/
		void begin(int inPin, PIN_TYPE inType);
		
		/**Sets the current speed of the port.
		@param inSpeed new current speed of the port.
		@return Previous speed.
		@remark Speed is only available on ANALOG or ANALOG_INVERTED type of pin.
		*/
		int SetSpeed(int inSpeed);

		/**Checks if this port is active. To be active, begin() must have been called.
		@return true if the pin number is different than -1.
		*/
		inline bool IsActive() const { return this->pin > -1; }
		/**Gets the Arduino pin.
		@return Pin number in Arduino mode.
		*/
		inline int GetPin() const { if (this->GetPinType() < ANALOG) return GPIO_to_Arduino_pin((GPIO_pin_t)this->pin); return this->pin;  }
		/**Gets the Arduino pin.
		@return Pin number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPin() const { if (this->GetPinType() >= ANALOG) return (GPIO_pin_t)this->pin; return DP_INVALID; }
		
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
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
