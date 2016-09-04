//-------------------------------------------------------------------
#ifndef __driverPortArduino_H__
#define __driverPortArduino_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

#ifndef NO_ARDUINODRIVER
class DriverPortArduino : public DriverPort
{
	protected:
		GPIO_pin_t pin;
		PORT_TYPE portType;
		int GetAnalogValue(int inValue) const;

	public:
		DriverPortArduino(uint8_t inId);
		
		virtual void begin(int inPin, PORT_TYPE inType = DIGITAL);
		
		inline bool IsActive() const { return this->pin != (GPIO_pin_t) DP_INVALID; }
		inline int GetPin() const { return GPIO_to_Arduino_pin(this->pin); }
		inline GPIO_pin_t GetPin2() const { return this->pin; }
		
		inline PORT_TYPE GetPortType() const { return this->portType; }

		int SetSpeed(int inSpeed);

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
