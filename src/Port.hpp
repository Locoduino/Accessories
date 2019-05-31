//-------------------------------------------------------------------
#ifndef __port_H__
#define __port_H__
//-------------------------------------------------------------------

#define DEFAULTSPEED	255
#define DEFAULTDURATION	100

/**	Describes the type of an Arduino pin.*/
enum PIN_TYPE 
{ 
	UNDEFINED = 0,			/**< Should never appear.*/
	DIGITAL = 1,			/**< Digital pin*/
	DIGITAL_INVERTED = 2,	/**< Inverted digital pin, using a pull-up resistor.*/
	ANALOG = 11,			/**< Analog pin.*/
	ANALOG_INVERTED = 12	/**< Analog pin using an inverted range of values.*/
};

/**	Describes the state of a port.*/
enum PORT_STATE 
{ 
	PORT_STOP,		/**< The port is topped.*/
	PORT_LEFT,		/**< Port on left position.*/
	PORT_RIGHT 		/**< Port on right position.*/
};

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
#define CHECKPIN(val, type, text)		Port::CheckPinNb(val, type, text)
#else
#define CHECKPIN(val, type, text)		Port::CheckPinNb(val, type, F(text))
#endif
#else
#define CHECKPIN(val, type, text)
#endif

/** A port is the way for an accessory to get current for power source.
The port gives an interface by pins to allow the power source to distribute or not its current.

A port can drive ANALOG or DIGITAL pins, with an inverted way by using the pull-up resistor of the pins.
Internally, an activated pin has always a state of HIGH. 
The MapValue() function will convert physical states to logical -library- states:

pin type         | physical state  | logical/analog state after MapValue
-----------------|-----------------|-------------------------------------
DIGITAL          |  HIGH           |  HIGH
DIGITAL          |  LOW            |  LOW
DIGITAL_INVERTED |  HIGH           |  LOW
DIGITAL_INVERTED |  LOW            |  HIGH
ANALOG           |  255            |  255
ANALOG           |  0              |  0
ANALOG_INVERTED  |  255            |  0
ANALOG_INVERTED  |  0              |  255


A port have a current state : left, right or stop, and a speed giving the amount of voltage used on the pins.

This is a base class for all kind of ports, DO NOT INSTANCIATE IT !
*/ 
class Port
{
	protected:
		/** Speed of the port.*/
		int speed;

		/** Kind of pin, analog or digital, inverted or not. and Current state of the port.
		Both values are stored inside the same byte of the form B00sskkkk
		where ss is two bits defining the state, and kkkk four bits for the kind of pin.
		The two last bits 00 are not used for the moment.
		*/
		byte type_state;


public:
		/**Gets the pin type.
		*/
		PIN_TYPE GetPinType() const { return (PIN_TYPE)(type_state & B00001111); }
		/**Gets the current port state.
		*/
		PORT_STATE GetPortState() const { return (PORT_STATE)((type_state >> 4) & B00000011); }

		/** Sets the pin type...
		*/
		void SetPinType(PIN_TYPE inType) { type_state = (int)inType | this->GetPortState() << 4; }
		/** Sets the port state.
		*/
		void SetPortState(PORT_STATE inState) { type_state = this->GetPinType() | (int)inState << 4; }

		/**Maps the given value to the kind of pin, inverted or not.
		According to the INVERTED flag or not, activated pin can be LOW or HIGH for the Arduino...
		The goal of this function is to assure that an activated pin is always HIGH for the library.
		@param inValue physical state of the pin : HIGH or LOW.
		@param inType if UNDEFINED, the pin type is this->pinType, but if necessary the type (for this function only) 
		can be forced to be inType.
		@return Logical pin state. HIGH or LOW for digital pin, LOW or HIGH for digit inverted pin.
		*/
		int MapValue(int inValue, PIN_TYPE inType = UNDEFINED) const;
		/** Initialize the instance of the pin.
		@param inPin pin to initialize.
		@param inType pin type
		@return Arduino pin number in analog mode, or DIO2 pin number for a digital pin.
		*/	    
		int beginPin(int inPin, PIN_TYPE inType = UNDEFINED) const;
		/**Sets the pin in a particular state. Accessories are not influenced by this change.
		@param inPin pin to update.
		@param inValue new value to reach.
		@param inType if UNDEFINED, the pin type is this->pinType, but if necessary the type (for this function only)
		can be forced to be inType.
		*/
		void MovePin(int inPin, int inValue, PIN_TYPE inType = UNDEFINED) const;

	public:
		/**Default constructor.*/
		Port();
		
		/** Initialize the instance.*/
		inline virtual void begin() {}
		/** Initialize the instance.
		Variant with a starting position.
		@param inStartingPosition Starting position.
		@remark The accessory will move to its starting position, it is not only a way to set internal data...
		*/
		inline virtual void beginByAccessory(int inStartingPosition) {}

		/**Gets the current speed of the port.
		@return current speed of the port.
		@remark Speed is only available on ANALOG or ANALOG_INVERTED type of pin.
		*/
		inline int GetSpeed() const { return this->speed; }
		/**Sets the current speed of the port.
		@param inSpeed new current speed of the port.
		@return Previous speed.
		@remark Speed is only available on ANALOG or ANALOG_INVERTED type of pin.
		*/
		virtual int SetSpeed(int inSpeed);
		
		/**Checks if the current state of the port is PORT_LEFT.
		@return true if the current state of the port is PORT_LEFT.
		*/
		inline bool IsLeftDir() const { return this->GetPortState() == PORT_LEFT; }
		/**Checks if the current state of the port is PORT_RIGHT.
		@return true if the current state of the port is PORT_RIGHT.
		*/
		inline bool IsRightDir() const { return this->GetPortState() == PORT_RIGHT; }
		/**Checks if the current state of the port is PORT_STOP.
		@return true if the current state of the port is PORT_STOP.
		*/
		inline bool IsStopped() const { return this->GetPortState() == PORT_STOP; }
		
		/**Sets the current state of the port to PORT_LEFT.
		@param inDuration the pins will be activated to left for the given delay.
		*/
		inline virtual void MoveLeftDir(unsigned long inDuration = 0) { this->SetPortState(PORT_LEFT); }
		/**Sets the current state of the port to PORT_RIGHT.
		@param inDuration the pins will be activated to right for the given delay.
		*/
		inline virtual void MoveRightDir(unsigned long inDuration = 0) { this->SetPortState(PORT_RIGHT); }
		/**Sets the current state of the port to PORT_LEFT for a given duration, at the given speed.
		@param inDuration the pins will be activated to left for the given delay.
		@param inSpeed the pins will be activated with the given speed.
		@remark This function does not change the current speed of the port.
		Speed is only available on ANALOG or ANALOG_INVERTED type of pin.
		*/
		void MoveLeftDir(unsigned long inDuration, int inSpeed);
		/**Sets the current state of the port to PORT_RIGHT for a given duration, at the given speed.
		@param inDuration the pins will be activated to left for the given delay.
		@param inSpeed the pins will be activated with the given speed.
		@remark This function does not change the current speed of the port.
		Speed is only available on ANALOG or ANALOG_INVERTED type of pin.
		*/
		void MoveRightDir(unsigned long inDuration, int inSpeed);
		/**Sets the current state of the port to PORT_LEFT or PORT_RIGHT according to its current state for a given duration.
		@param inDuration the pins will be activated to left for the given delay.
		*/
		PORT_STATE MoveToggle(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_STOP.	*/
		inline virtual void MoveStop()	{ this->SetPortState(PORT_STOP); }
		/**Move the port to the given position using a given duration.
		@param inDuration the movement should take the time of this delay.
		@param inEndPosition final position to reach.
		@remark This function is only available on relevant accessories, like stepper or servo.
		*/
		inline virtual void MovePosition(unsigned long inDuration, int inEndPosition) {}
		/**Gets the current position of the port.
		@return Current position of the port.
		@remark This function is only available on relevant accessories, like stepper or servo.
		*/
		inline virtual int GetPosition() { return 0; }

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
		static void CheckPinNb(int inPin, const char *infunc);
		static void CheckDIOPinNb(GPIO_pin_t inPin, const char *infunc);
#else
		static void CheckPinNb(int inPin, PIN_TYPE inType, const __FlashStringHelper *infunc);
#endif
#endif
#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/** Print one port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		virtual void printPort();
		/** Print a pin on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		static void printPortPin(int inPin, PIN_TYPE inType);
#endif
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
