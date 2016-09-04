//-------------------------------------------------------------------
#ifndef __driverPortLMD18200_H__
#define __driverPortLMD18200_H__
//-------------------------------------------------------------------

#include "DriverPort.hpp"

//-------------------------------------------------------------------

class DriverPortLMD18200 : public DriverPort
{
	protected:
		int pinPWM;
		GPIO_pin_t pinDir;
		GPIO_pin_t pinBrake;

	public:
		DriverPortLMD18200(uint8_t inId);
		
		void begin(int inPinPWM, int pinDir, int inPinBrake);
		
		inline bool IsActive() const { return this->pinPWM > (int) (GPIO_pin_t) DP_INVALID; }
		inline int GetPinPWM() const { return this->pinPWM; }
		inline int GetPinDir() const { return GPIO_to_Arduino_pin(this->pinDir); }
		inline GPIO_pin_t GetPinDir2() const { return this->pinDir; }
		inline int GetPinBrake() const { return GPIO_to_Arduino_pin(this->pinBrake); }
		inline GPIO_pin_t GetPinBrake2() const { return this->pinBrake; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
