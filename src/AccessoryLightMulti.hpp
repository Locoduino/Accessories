//-------------------------------------------------------------------
#ifndef __accessoryLightMulti_H__
#define __accessoryLightMulti_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
#include "AccessoryBaseLight.hpp"
#include "Driver.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

// This class describes a basic light.

#define LIGHTMULTI(list, nb)	((AccessoryLightMulti *) (list)[nb])

class AccessoryLightMulti : public Accessory
{
	private:
		// Dont use a chained list here, because the user only gives a number. He does not add lights one by one...
		AccessoryBaseLight *pLights;
		int *pMovingPositionBlinks;
		uint8_t lightsSize;

	public:
		AccessoryLightMulti();
		
		void begin(unsigned long inId, uint8_t inSize, unsigned long inBlinkDuration);
		void beginLight(uint8_t inIndex, DriverPort *inpPort, int inIntensity = 255);

		inline bool IsOn(uint8_t inIndex) const { return this->pLights[inIndex].IsOn(); }
		inline bool IsFlashing(uint8_t inIndex) const { return this->pLights[inIndex].IsBlinking(); }
		inline bool IsFading(uint8_t inIndex) const { return this->pLights[inIndex].IsFading(); }
		inline uint8_t GetSize() const { return this->lightsSize; }

		unsigned char AddMovingPosition(unsigned long inId, int inOnMask, int inBlinkMask);
		inline void SetBlinking(uint8_t inIndex, unsigned long inBlinkingDelay) { this->pLights[inIndex].SetBlinking(inBlinkingDelay); }
		inline void SetFading(uint8_t inIndex, uint8_t inStep, uint8_t inDelay) { this->pLights[inIndex].SetFading(inStep, inDelay); }
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		inline void LightOn(uint8_t inIndex) { this->pLights[inIndex].LightOn(); }
		inline void LightOff(uint8_t inIndex) { this->pLights[inIndex].LightOff(); }
		inline void Blink(uint8_t inIndex) { this->pLights[inIndex].Blink(); }

		void LightOn();
		void LightOff();
		void Blink();

	private:
		inline void SetState(uint8_t inIndex, ACC_STATE inState) { this->pLights[inIndex].SetState(inState); }
		inline ACC_STATE Toggle(uint8_t inIndex) { return this->pLights[inIndex].Toggle(); }

		inline bool IsGroupActionPending(uint8_t inIndex) { return this->pLights[inIndex].IsGroupActionPending(); }
		inline void StartAction(uint8_t inIndex) { this->pLights[inIndex].StartAction(); }
		inline bool ActionEnded(uint8_t inIndex) { return this->pLights[inIndex].ActionEnded(); }

		bool ActionEnded();

		ACC_STATE Toggle();
		void Move(unsigned long inId);
		void Move(int position);
		void MoveBlink(int inOnMask, int inBlinkMask);
#ifdef ACCESSORIES_DEBUG_MODE
		void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
