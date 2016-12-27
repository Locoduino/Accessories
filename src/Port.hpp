//-------------------------------------------------------------------
#ifndef __port_H__
#define __port_H__
//-------------------------------------------------------------------

#define DEFAULTSPEED	255
#define DEFAULTDURATION	100

enum PIN_TYPE { UNDEFINED = 0, DIGITAL = 1, DIGITAL_INVERTED = 2, ANALOG = 11, ANALOG_INVERTED = 12 };
enum PORT_STATE { PORT_STOP, PORT_LEFT, PORT_RIGHT };

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
#define CHECKPIN(val, type, text)		Port::CheckPinNb(val, type, text)
#else
#define CHECKPIN(val, type, text)		Port::CheckPinNb(val, type, F(text))
#endif
#else
#define CHECKPIN(val, type, text)
#endif

class Port
{
	protected:
		PIN_TYPE pinType;
		PORT_STATE state;
		int speed;

		int MapValue(int inValue, PIN_TYPE inType = UNDEFINED) const;
		int beginPin(int inPin, PIN_TYPE inType = UNDEFINED) const;
		void MovePin(int inPin, int inValue, PIN_TYPE inType = UNDEFINED) const;

	public:
		Port();
		
		inline virtual void begin() {}
		inline virtual void beginByAccessory(int inStartingPosition) {}

		inline PORT_STATE GetState() const { return this->state; }
		inline int GetSpeed() const { return this->speed; }
		virtual int SetSpeed(int inSpeed);
		
		inline bool IsLeftDir() const { return this->state == PORT_LEFT; }
		inline bool IsRightDir() const { return this->state == PORT_RIGHT; }
		inline bool IsStopped() const { return this->state == PORT_STOP; }
		
		inline virtual void MoveLeftDir(unsigned long inDuration = 0) { this->state = PORT_LEFT; }
		inline virtual void MoveRightDir(unsigned long inDuration = 0) { this->state = PORT_RIGHT; }
		void MoveLeftDir(unsigned long inDuration, int inSpeed);
		void MoveRightDir(unsigned long inDuration, int inSpeed);
		PORT_STATE MoveToggle(unsigned long inDuration = 0);
		inline virtual void MoveStop()	{ this->state = PORT_STOP; }
		inline virtual void MovePosition(unsigned long inDuration, int inEndPosition) {}
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
		virtual void printPort();
		static void printPortPin(int inPin, PIN_TYPE inType);
#endif
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
