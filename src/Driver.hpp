//-------------------------------------------------------------------
#ifndef __driver_H__
#define __driver_H__
//-------------------------------------------------------------------

enum PORTTYPE
{
	MOTOR_LIGHT = 0,
	SERVO = 1,
	STEPPER = 2
};

#include "DriverPort.hpp"
#include "Chain.hpp"

#define PORT_UNDEFINED	-1

#ifdef ACCESSORIES_DEBUG_MODE
	#ifdef ARDUINO_ARCH_SAM
		#define CHECKPIN(val, text)		Driver::CheckPinNb(val, text)
	#else
		#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
	#endif
#else
#define CHECKPIN(val, text)
#endif

class Driver
{
	private:
		static Driver *pFirstDriver;
		static void AddDriver(Driver *inDriver);
		static uint8_t idCounter;

		Driver *pNextDriver;

	protected:
		ACCSCHAINEDLIST<DriverPort> Ports;
	
	public:
		Driver();
		
		inline PORT_STATE GetState(PORTTYPE inType, uint8_t inPort) const { return this->GetPort(inType,inPort)->GetState(); }
		inline int GetSpeed(PORTTYPE inType, uint8_t inPort) const { return this->GetPort(inType, inPort)->GetSpeed(); }
		int SetSpeed(PORTTYPE inType, uint8_t inPort, int inSpeed);
		DriverPort *GetPort(PORTTYPE inType, uint8_t inPort) const;

	protected:
		void AddPort(DriverPort *inpPort);

	public:
		virtual void begin();
		virtual void beginByAccessory(PORTTYPE inType, uint8_t inPort, int inStartingPosition);

		void MoveLeftDir(PORTTYPE inType, uint8_t inPort, unsigned long inDuration = 0, int inSpeed = 0);
		void MoveRightDir(PORTTYPE inType, uint8_t inPort, unsigned long inDuration = 0, int inSpeed = 0);
		PORT_STATE MoveToggle(PORTTYPE inType, uint8_t inPort, unsigned long inDuration = 0, int inSpeed = 0);
		inline void MoveStop(PORTTYPE inType, uint8_t inPort) { this->GetPort(inType, inPort)->MoveStop(); }
		inline void MovePosition(PORTTYPE inType, uint8_t inPort, unsigned long inDuration, int inEndPosition) {
			this->GetPort(inType, inPort)->MovePosition(inDuration, inEndPosition); }
		inline int GetPosition(PORTTYPE inType, uint8_t inPort) { return this->GetPort(inType, inPort)->GetPosition(); }

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ARDUINO_ARCH_SAM
		static void CheckPinNb(int inPin, const char *infunc);
		static void CheckPinNb2(GPIO_pin_t inPin, const char *infunc);
#else
		static void CheckPinNb(int inPin, const __FlashStringHelper *infunc);
		static void CheckPinNb2(GPIO_pin_t inPin, const __FlashStringHelper *infunc);
#endif
#endif
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
