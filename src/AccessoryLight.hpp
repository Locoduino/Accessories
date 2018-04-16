//-------------------------------------------------------------------
#ifndef __accessoryLight_H__
#define __accessoryLight_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"
#include "AccessoryBaseLight.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

/** This class describes a single light accessory.

Events handled:

id       |         type          | data| effect
---------|-----------------------|-----|--------------------
light id|ACCESSORIES_EVENT_MOVEPOSITIONID||toggle on/off
light id      | ACCESSORIES_EVENT_TOGGLE |  | toggle on/off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STRAIGHT | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_TOP | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LEFT | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_DIVERGE | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_BOTTOM | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_RIGHT | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_ON | Light on
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STOP | Light off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_OFF | Light off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_MORE| Increase speed of 1
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LESS| Decrease speed of 1
light id      | ACCESSORIES_EVENT_MOVEPOSITION | New speed | Set the light On at the given speed, without changing the current speed.
light id      | ACCESSORIES_EVENT_SETSPEED | New speed | Change the current speed
*/

class AccessoryLight : public Accessory
{
	private:
		AccessoryBaseLight *pLight;

	public:
		/**Default constructor.*/
		AccessoryLight();

		/** Initialize the instance.
		@param inpPort Port driven this light.
		@param inId Id of this accessory.
		@param inBlinkDuration Blink duration, 0 for fix light. Default is 0.
		@param inIntensity Intensity of this light. Default is maximum, 255.
		*/
		void begin(Port *inpPort, unsigned long inId, unsigned long inBlinkDuration = 0, int inIntensity = 255);
		/**Sets the blinking delay.
		@param inBlinkingDelay Blinking delay in milliseconds.
		*/
		inline void SetBlinking(unsigned long inBlinkingDelay) { this->pLight->SetBlinking(inBlinkingDelay); }
		/**Set the fading mode, defining its speed.
		@param inStep Number of steps between light on and light off.
		@param inDelay Duration of each step.
		*/
		inline void SetFading(uint8_t inStep, uint8_t inDelay) { this->pLight->SetFading(inStep, inDelay); }

		/**Checks if the light is on.
		@return true if the light is on.
		*/
		inline bool IsOn() const { return this->pLight->IsOn(); }
		/**Checks if the light is blinking.
		@return true if the light is blinking.
		*/
		inline bool IsBlinking() const { return this->pLight->IsBlinking(); }
		/**Checks if the light is fading.
		@return true if the light is fading.
		@remark A light can dim only with an analog pin.
		*/
		inline bool IsFading() const { return this->pLight->IsFading(); }

		/** Execute a new event.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		/**Sets the internal state.
		@param inState new state LIGHTON, LIGHTOFF or LIGHTBLINKING.
		*/
		inline void SetState(ACC_STATE inState) { this->pLight->SetState(inState); }
		/**Change the state from ON to OFF or OFF to ON.
		@return The new state.
		*/
		inline ACC_STATE Toggle() { return this->pLight->Toggle(); }
		/**Turn the light on.*/
		inline void LightOn() { this->pLight->LightOn(); }
		/**Turn the light off.*/
		inline void LightOff() { this->pLight->LightOff(); }
		/**Turn the light on and blink.*/
		inline void Blink() { this->pLight->Blink(); }

#ifndef NO_EEPROM
		/**Reload all data from the EEPROM.
		@remark Only for internal usage.
		*/
		int EEPROMLoad(int inPos);
#endif

	private:
		inline bool IsGroupActionPending() { return this->pLight->IsGroupActionPending(); }
		inline void StartAction() { this->pLight->StartAction(); }
		inline bool ActionEnded() { return this->pLight->ActionEnded(); }
		inline void ResetAction() { return this->ResetStartingMillis(); }

		void Move(unsigned long inId);

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print this accessory on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
