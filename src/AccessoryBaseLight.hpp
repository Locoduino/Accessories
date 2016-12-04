//-------------------------------------------------------------------
#ifndef __accessoryBaseLight2_H__
#define __accessoryBaseLight2_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

#define LIGHTON		STATE_FIRST
#define LIGHTBLINK	STATE_SECOND
#define LIGHTOFF	STATE_NONE

#define FADING_FULL_DELAY	(this->fadingStep == 0 ? 0 : ((255UL / this->fadingStep)+1) * this->fadingDelay)

// This class describes a basic light.

class AccessoryBaseLight
{
	private:
		// Fix data from creator
		Port *pPort;
		uint8_t fadingStep;
		uint8_t fadingDelay;
		unsigned long blinkingDelay;

		// State of the light
		ACC_STATE state;
		enum LightState { LIGHT_ON, LIGHT_OFF, LIGHT_ASCENDING, LIGHT_DESCENDING };
		LightState currentState;
		uint8_t fadingCurrentValue;
		unsigned long startingMillis;

		friend class AccessoryLight;
		friend class AccessoryLightMulti;

		Accessory *pOwner;

	public:
		AccessoryBaseLight(Accessory *inpOwner = 0);
		
		void begin(Port *inpPort, int inIntensity = 255, Accessory *inpOwner = 0);

		inline Port *GetPort() const { return this->pPort; }
		inline bool IsOn() const { return this->state == LIGHTON; }
		inline bool IsBlinking() const { return this->state == LIGHTBLINK; }
		inline bool IsFading() const { return this->fadingStep > 0; }

		void Event(ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
		inline void SetBlinking(unsigned long inBlinkingDelay) { this->blinkingDelay = inBlinkingDelay; }
		void SetFading(uint8_t inStep, uint8_t inDelay);

	private:
		void SetState(ACC_STATE instate);
		void SetStateRaw(ACC_STATE instate);
		ACC_STATE Toggle();
		void LightOn();
		void LightOff();
		inline void Blink() { this->SetState(LIGHTBLINK); this->StartAction(); }

		inline bool IsGroupActionPending() const	{ return false; }
		inline unsigned long GetActionStartingMillis() const { return this->startingMillis; }
		inline void ResetAction() { this->startingMillis = 0; }
		void StartAction();
		bool ActionEnded();

		void LightOnRaw();
		void LightOffRaw();
		void LightFadingRaw(uint8_t inValue);
#ifndef NO_EEPROM
		int EEPROMSave(int inPos, bool inSimulate = false);
		int EEPROMLoad(int inPos);
#endif
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
