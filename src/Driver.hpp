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

#ifdef DEBUG_MODE
#define CHECKPIN(val, text)		Driver::CheckPinNb(val, F(text))
#else
#define CHECKPIN(val, text)
#endif

class Driver
{
	private:
		static Driver *pFirstDriver;
		static void AddDriver(Driver *inDriver);
		static byte idCounter;

		Driver *pNextDriver;

	protected:
		ACCSCHAINEDLIST<DriverPort> Ports;
	
	public:
		Driver();
		
		inline PORT_STATE GetState(PORTTYPE inType, byte inPort) const { return this->GetPort(inType,inPort)->GetState(); }
		inline int GetSpeed(PORTTYPE inType, byte inPort) const { return this->GetPort(inType, inPort)->GetSpeed(); }
		int SetSpeed(PORTTYPE inType, byte inPort, int inSpeed);
		DriverPort *GetPort(PORTTYPE inType, byte inPort) const;

	protected:
		void AddPort(DriverPort *inpPort);

	public:
		virtual void begin();
		virtual void beginByAccessory(PORTTYPE inType, byte inPort, int inStartingPosition);

		void MoveLeftDir(PORTTYPE inType, byte inPort, unsigned long inDuration = 0, int inSpeed = 0);
		void MoveRightDir(PORTTYPE inType, byte inPort, unsigned long inDuration = 0, int inSpeed = 0);
		PORT_STATE MoveToggle(PORTTYPE inType, byte inPort, unsigned long inDuration = 0, int inSpeed = 0);
		inline void MoveStop(PORTTYPE inType, byte inPort) { this->GetPort(inType, inPort)->MoveStop(); }
		inline void MovePosition(PORTTYPE inType, byte inPort, unsigned long inDuration, int inEndPosition) {
			this->GetPort(inType, inPort)->MovePosition(inDuration, inEndPosition); }
		inline int GetPosition(PORTTYPE inType, byte inPort) { return this->GetPort(inType, inPort)->GetPosition(); }

		static void loops();

#ifdef DEBUG_MODE
		static void CheckPinNb(int inPin, const __FlashStringHelper *infunc);
		static void CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *infunc);
#endif
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
