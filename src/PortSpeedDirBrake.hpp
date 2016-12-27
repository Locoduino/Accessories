//-------------------------------------------------------------------
#ifndef __PortPWMDirBrake_H__
#define __PortPWMDirBrake_H__
//-------------------------------------------------------------------

#include "Port.hpp"

//-------------------------------------------------------------------

class PortSpeedDirBrake : public Port
{
	protected:
		int pinPWM;
		PIN_TYPE digitalType;
		GPIO_pin_t pinDir;
		GPIO_pin_t pinBrake;

		int MapDigitalValue(int inValue) const;

	public:
		PortSpeedDirBrake();
		
		void begin(int inPinPWM, int pinDir, int inPinBrake, PIN_TYPE inPWMType, PIN_TYPE inDigitalType);
		
		inline bool IsActive() const { return this->pinPWM > (int) (GPIO_pin_t) DP_INVALID; }
		inline int GetPinPWM() const { return this->pinPWM; }
		inline int GetPinDir() const { return GPIO_to_Arduino_pin(this->pinDir); }
		inline GPIO_pin_t GetDIOPinDir() const { return this->pinDir; }
		inline int GetPinBrake() const { return GPIO_to_Arduino_pin(this->pinBrake); }
		inline GPIO_pin_t GetDIOPinBrake() const { return this->pinBrake; }

		void MoveLeftDir(unsigned long inDuration = 0);
		void MoveRightDir(unsigned long inDuration = 0);
		void MoveStop();
#ifdef ACCESSORIES_PRINT_ACCESSORIES
		void printPort();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
