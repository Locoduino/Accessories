//-------------------------------------------------------------------
#ifndef __accessoryBaseLight2_H__
#define __accessoryBaseLight2_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

/** Re-define STATE_FIRST to LIGHTON.*/
#define LIGHTON		STATE_FIRST
/** Re-define STATE_SECOND to LIGHTBLINK.*/
#define LIGHTBLINK	STATE_SECOND
/** Re-define STATE_NONE to LIGHTOFF.*/
#define LIGHTOFF	STATE_NONE

/** Computes the total duration of a fading cycle.*/
#define FADING_FULL_DELAY	(this->fadingStep == 0 ? 0 : ((255UL / this->fadingStep)+1) * this->fadingDelay)

/** This class describes a proxy for a basic light.
This is the way to share identical functions and behaviors for many kind of light accessory.
A light basically can be on, off, blinking and / or fading (dim light).
*/
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
		/**Default constructor.*/
		AccessoryBaseLight(Accessory *inpOwner = 0);
		
		/** Initialize the instance.
		@param inpPort Port driven this light.
		@param inIntensity Intensity of this light. Default is maximum, 255.
		@param inpOwner Address of the real Accessory owner of this proxy.
		*/
		void begin(Port *inpPort, int inIntensity = 255, Accessory *inpOwner = 0);

		/**Gets the associated port.
		@return port address or NULL.
		*/
		inline Port *GetPort() const { return this->pPort; }
		/**Checks if the light is on.
		@return true if the light is on.
		*/
		inline bool IsOn() const { return this->state == LIGHTON; }
		/**Checks if the light is blinking.
		@return true if the light is blinking.
		*/
		inline bool IsBlinking() const { return this->state == LIGHTBLINK; }
		/**Checks if the light is fading.
		@return true if the light is fading.
		@remark A light can dim only with an analog pin.
		*/
		inline bool IsFading() const { return this->fadingStep > 0; }

		/** Execute a new event.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);
		/**Set the blinking mode, with a duration for a blink.
		@param inBlinkingDelay Duration of the light on state. The same duration is applied for light off.
		*/
		inline void SetBlinking(unsigned long inBlinkingDelay) { this->blinkingDelay = inBlinkingDelay; }
		/**Set the fading mode, defining its speed.
		@param inStep Number of steps between light on and light off.
		@param inDelay Duration of each step.
		*/
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
