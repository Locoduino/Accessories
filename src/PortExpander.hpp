//-------------------------------------------------------------------
#ifndef __portexpander_H__
#define __portexpander_H__
//-------------------------------------------------------------------

#ifndef NO_EXPANDER

/**	This macro must be called to give a pin number from an expander, instead of an Arduino pin number.
The expander id is encoded into the integer of the pin, and must be between 1 and 32.
The expander pin number is also encoded inside the integer of the pin. It must be between 0 and 999.
*/
#define	EXPANDER_PIN(expid, pin)		(((expid) * 1000) + pin)
/** Extract the pin number from an encoded expander pin number. */
#define EXPANDER_PORT_PIN(pinnb)		((pinnb) % 1000)
/** Extract the expander id from an encoded expander pin number. */
#define EXPANDER_PORT_EXPID(pinnb)	((pinnb) / 1000)

/** This class describes an expander : a circuit wich provides a lot of input/output pin
for a small number of pins on the Arduino. Each expander is identified by a number : the id.
Only output pins are used in Accessories. An expander can provide digital or analog (PWM) output.*/
class PortExpander
{
	private:
		static PortExpander *pFirstExpander;
		PortExpander *pNextExpander;

		inline void SetNextExpander(PortExpander *inExpander) { this->pNextExpander = inExpander; }
		inline PortExpander *GetNextExpander() const { return this->pNextExpander; }
		static PortExpander *GetById(int inId);

	protected:
		static void Add(PortExpander* inExpander);

		int id;

		/** Initialize the instance of the expander.
		@param inId id of expander (to be used associated to pin).
		@return Is expander initialized ?
		*/
		inline virtual byte begin() { return 0; }
		/** Initialize one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inType type of the output of this pin : DIGITAL or ANALMOG (pwm).
		*/
		virtual void beginPin(int inPin, PIN_TYPE inType) = 0;
		/** Set the state of one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inValue New state of the pin. Should be HIGH (1) or LOW (0).
		*/
		virtual void digitalWrite(int inPin, int inValue) = 0;
		/** Set the value of one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inValue New analog value of the pin. Should be between 0 and 255.
		*/
		virtual void analogWrite(int inPin, int inValue) = 0;
		/** Gets the total number of pins of the expander.
		@return Number of pins.
		*/
		inline virtual byte GetPinsNumber() { return 0; }

	public:
		/**Default constructor.*/
		PortExpander();

		/** Initialize one pin of the expander.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inExpId id of the expander. should be a valid expander id.
		@param inType type of the output of this pin : DIGITAL or ANALMOG (pwm).
		*/
		static void beginPin(int inPin, int inExpId, PIN_TYPE inType);
		/** Set the value of one pin of the given expander ID.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inExpId id of the expander. should be a valid expander id.
		@param inValue New state of the pin. Should be HIGH (1) or LOW (0).
		*/
		static void digitalWrite(int inPin, int inExpId, int inValue);
		/** Set the value of one pin of the given expander ID.
		@param inPin number of the pin. should be between 0 and the maximum number of expander pins.
		@param inExpId id of the expander. should be a valid expander id.
		@param inValue New analog value of the pin. Should be between 0 and 255.
		*/
		static void analogWrite(int inPin, int inExpId, int inValue);


#ifdef ACCESSORIES_DEBUG_MODE
		static void CheckAllExpanderPins();
		static void CheckExpanderId(int inId);
#endif
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
