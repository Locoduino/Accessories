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
		byte lightsSize;

	public:
		AccessoryLightMulti();
		
		void begin(unsigned long inId, byte inSize, unsigned long inBlinkDuration);
		void beginLight(byte inIndex, DriverPort *inpPort, int inIntensity = 255);

		inline bool IsOn(byte inIndex) const { return this->pLights[inIndex].IsOn(); }
		inline bool IsFlashing(byte inIndex) const { return this->pLights[inIndex].IsBlinking(); }
		inline bool IsFading(byte inIndex) const { return this->pLights[inIndex].IsFading(); }
		inline byte GetSize() const { return this->lightsSize; }

		unsigned char AddMovingPosition(unsigned long inId, int inOnMask, int inBlinkMask);
		inline void SetBlinking(byte inIndex, unsigned long inBlinkingDelay) { this->pLights[inIndex].SetBlinking(inBlinkingDelay); }
		inline void SetFading(byte inIndex, byte inStep, byte inDelay) { this->pLights[inIndex].SetFading(inStep, inDelay); }
		inline void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_TOGGLE, int inData = 0) { this->pLights->Event(inEvent, inData); }

		inline void LightOn(byte inIndex) { this->pLights[inIndex].LightOn(); }
		inline void LightOff(byte inIndex) { this->pLights[inIndex].LightOff(); }
		inline void Blink(byte inIndex) { this->pLights[inIndex].Blink(); }

		void LightOn();
		void LightOff();
		void Blink();

	private:
		inline void SetState(byte inIndex, ACC_STATE inState) { this->pLights[inIndex].SetState(inState); }
		inline ACC_STATE Toggle(byte inIndex) { return this->pLights[inIndex].Toggle(); }

		inline bool IsGroupActionPending(byte inIndex) { return this->pLights[inIndex].IsGroupActionPending(); }
		inline void StartAction(byte inIndex) { this->pLights[inIndex].StartAction(); }
		inline bool ActionEnded(byte inIndex) { return this->pLights[inIndex].ActionEnded(); }

		bool ActionEnded();

		ACC_STATE Toggle();
		void Move(unsigned long inId);
		void Move(int position);
		void MoveBlink(int inOnMask, int inBlinkMask);
#ifdef DEBUG_MODE
		void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
