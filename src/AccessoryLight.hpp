//-------------------------------------------------------------------
#ifndef __accessoryLight_H__
#define __accessoryLight_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
#include "AccessoryBaseLight.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

#define LIGHTON		STATE_FIRST
#define LIGHTBLINK	STATE_SECOND
#define LIGHTOFF	STATE_NONE

// This class describes a single light accessory.

class AccessoryLight : public Accessory
{
	private:
		AccessoryBaseLight *pLight;

	public:
		AccessoryLight();

		void begin(DriverPort *inpPort, unsigned long inId, unsigned long inBlinkDuration = 0, int inIntensity = 255);
		inline void SetFading(uint8_t inStep, uint8_t inDelay) { this->pLight->SetFading(inStep, inDelay); }

		inline bool IsOn() const { return this->pLight->IsOn(); }
		inline bool IsFlashing() const { return this->pLight->IsBlinking(); }
		inline bool IsFading() const { return this->pLight->IsFading(); }

		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		inline void SetState(ACC_STATE inState) { this->pLight->SetState(inState); }
		inline ACC_STATE Toggle() { return this->pLight->Toggle(); }
		inline void LightOn() { this->pLight->LightOn(); }
		inline void LightOff() { this->pLight->LightOff(); }
		inline void Blink() { this->pLight->Blink(); }

	private:
		inline bool IsGroupActionPending() { return this->pLight->IsGroupActionPending(); }
		inline void StartAction() { this->pLight->StartAction(); }
		inline bool ActionEnded() { return this->pLight->ActionEnded(); }
		inline void ResetAction() { return this->ResetStartingMillis(); }

		void Move(unsigned long inId);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
