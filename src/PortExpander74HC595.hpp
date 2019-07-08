//-------------------------------------------------------------------
#ifndef __portexpander74HC595_H__
#define __portexpander74HC595_H__
//-------------------------------------------------------------------

#ifndef NO_EXPANDER_74HC595

#include "PortExpander.hpp"
#include "ShiftRegister74HC595.h" // Include 74HC595 library

/** This class describes a shift register 74HC595 as an expander: a circuit wich provides 8 pins per circuit, 
and which can be linked to another circuit to increase the number of output pins whitout increasing
the number of pins of the Arduino used ! Each expander is identified by a number : the id.
Only output pins are used in Accessories. This expander can only provide digital output.*/
class PortExpander74HC595: public PortExpander
{
	private:
		ShiftRegister74HC595 *psr; // Create an 74HC595 object to be used throughout
		byte numberOfShiftRegisters;

	public:
		/** Initialize the instance of the expander.
		@param inId											id of expander (to be used associated to pin).
		@param numberOfShiftRegisters		Number of linked modules. One is the minimum.
		@param serialDataPin pin used to send serial data to the chip.
		@param clockPin		Shift register clock pin
		@param latchPin		Storage register clock pin (latch pin)
		@return Always true.
		*/
		byte begin(int inID, int numberOfShiftRegisters, int serialDataPin, int clockPin, int latchPin);
		/** Initialize one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inType type of the output of this pin : only DIGITAL here.
		*/
		void beginPin(int inPin, PIN_TYPE inType);

		/** Set the state of one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inValue New state of the pin. Should be HIGH (1) or LOW (0).
		*/
		void digitalWrite(int inPin, int inValue);
		/** Set the value of one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inValue New analog value of the pin. Should be between 0 and 255.
		@remark No effect on this chip.
		*/
		void analogWrite(int inPin, int inValue);
		/** Gets the total number of pins.
		*/
		byte GetPinsNumber() { return 8 * this->numberOfShiftRegisters; }

	public:
		/**Default constructor.*/
		PortExpander74HC595();
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
