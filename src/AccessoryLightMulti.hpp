//-------------------------------------------------------------------
#ifndef __accessoryLightMulti_H__
#define __accessoryLightMulti_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#ifndef NO_LIGHT

#define LIGHTMULTI(list, nb)	((AccessoryLightMulti *) (list)[nb])

/**This class describes a group of lights.
This is typically used for railroad or traffic signals.
This group of lights can define a 'MovingPosition' which represents a combination of
lights on, off and/or blinking. Each moving position defines an identifier and 
two masks on an integer, one for on/off status, another optional one for blinking lights.
An integer is 16 bits long, so this is the maximum number of lights a 
MovingPosition can handle.

For a group of five lights, three moving positions defined with id from 125 to 127 (why not ?) :

 moving position id |  mask  | blink mask | light 0 | 1 | 2 | 3 | 4
--------------------|--------|------------|---------|---|---|---|---
125					| B10000 | B00000     |	  on	|off|off|off|off
126					| B10100 | B00000     |	  on	|off|on |off|off
127					| B11000 | B00001     |	  on	|on |off|off|blink

The moving positions are stored like any other Accessory in the MovingPosition list in 
the base class Accessory itself. The blinking masks cannot be stored in the same structure,
so a new list of blinking masks is created inside this class, and the indexes are 
synchronized between the two lists.

Events handled:

id       |         type          | data| effect
---------|-----------------------|-----|--------------------
moving position id|ACCESSORIES_EVENT_MOVEPOSITIONID||Use the id to reach this moving position
moving position id|ACCESSORIES_EVENT_MOVEPOSITIONINDEX|Index|Use the index to reach this moving position
light id      | ACCESSORIES_EVENT_TOGGLE |  | toggle all lights on/off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STRAIGHT | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_TOP | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LEFT | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_DIVERGE | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_BOTTOM | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_RIGHT | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_ON | All Lights On
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_STOP | All Lights Off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_OFF | All Lights Off
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_MORE| Increase speed of 1 for all ligths
light id      | ACCESSORIES_EVENT_MOVE | ACCESSORIES_MOVE_LESS| Decrease speed of 1 for all lights
light id      | ACCESSORIES_EVENT_MOVEPOSITION | New speed | Set the all the lights On at the given speed, without changing the current speed.
light id      | ACCESSORIES_EVENT_SETSPEED | New speed | Change the current speed for all lights
*/

class AccessoryLightMulti : public Accessory
{
	private:
		// Do not use a chained list here, because the user only gives a number. He does not add lights one by one...
		AccessoryBaseLight *pLights;
		int *pMovingPositionBlinks;
		uint8_t lightsSize;

	public:
		/**Default constructor.*/
		AccessoryLightMulti();
		
		/** Initialize the instance.
		@param inId Id of this accessory.
		@param inSize Total number of lights in the group.
		@param inBlinkDuration Blink duration, 0 for fix light. Default is 0.
		*/
		void begin(unsigned long inId, uint8_t inSize, unsigned long inBlinkDuration);
		/** Initialize one light in the group.
		@param inIndex index of the light in the group.
		@param inpPort Port driven this light.
		@param inIntensity Intensity of this light. Default is maximum, 255.
		*/
		void beginLight(uint8_t inIndex, Port *inpPort, int inIntensity = 255);

		/**Checks if the light is blinking.
		@param inIndex index of the light to check.
		@return true if the light is blinking.
		*/
		inline bool IsOn(uint8_t inIndex) const { return this->pLights[inIndex].IsOn(); }
		/**Checks if the light is blinking.
		@param inIndex index of the light to check.
		@return true if the light is blinking.
		*/
		inline bool IsBlinking(uint8_t inIndex) const { return this->pLights[inIndex].IsBlinking(); }
		/**Checks if the light is fading.
		@param inIndex index of the light to check.
		@return true if the light is fading.
		@remark A light can dim only with an analog pin.
		*/
		inline bool IsFading(uint8_t inIndex) const { return this->pLights[inIndex].IsFading(); }
		/**Gets the number of lights.
		@return number of lights.
		*/
		inline uint8_t GetSize() const { return this->lightsSize; }

		/**Add a new moving position.
		@param inId Id of this moving position.
		@param inOnMask Each bit at 1 of this mask will turn on this light on.
		@param inBlinkMask Each bit at 1 of this mask will flash this light. Default is 0.
		@return index of the moving position in the internal list.
		*/
		unsigned char AddMovingPosition(unsigned long inId, int inOnMask, int inBlinkMask = 0);
		/**Sets the number of moving positions. If some MovingPosition was previously existing, they will be preserved.
		@param inNewSize new size.
		*/
		void AdjustMovingPositionBlinksSize(uint8_t inNewSize);
		/**Gets the blinking mask for the given index.
		@param inIndex Index of the light to scan.
		@return Blinking mask for the light number 'inIndex'.
		*/
		inline int GetMovingPositionBlinksByIndex(int inIndex) const { return this->pMovingPositionBlinks[inIndex]; }
		/**Sets the blinking delay for the given index.
		@param inIndex Index of the light to scan.
		@param inBlinkingDelay Blinking delay in milliseconds for the light number 'inIndex'.
		*/
		inline void SetBlinking(uint8_t inIndex, unsigned long inBlinkingDelay) { this->pLights[inIndex].SetBlinking(inBlinkingDelay); }
		/**Sets the fading steps for the given index.
		@param inIndex Index of the light to scan.
		@param inStep Number of steps between light on and light off.
		@param inDelay Duration of each step.
		*/
		inline void SetFading(uint8_t inIndex, uint8_t inStep, uint8_t inDelay) { this->pLights[inIndex].SetFading(inStep, inDelay); }
		/** Execute a new event.
		@param inId Id of an accessory or an accessory item.
		@param inEvent Type of the new event. Default is ACCESSORIES_EVENT_MOVEPOSITIONID.
		@param inData Associated data to the event type. Default is 0.
		*/
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_MOVEPOSITIONID, int inData = 0);

		/**Turn the given light on.
		@param inIndex Index of the light to scan.
		*/
		inline void LightOn(uint8_t inIndex) { this->pLights[inIndex].LightOn(); }
		/**Turn the given light off.
		@param inIndex Index of the light to scan.
		*/
		inline void LightOff(uint8_t inIndex) { this->pLights[inIndex].LightOff(); }
		/**Turn the given light on and blink.
		@param inIndex Index of the light to scan.
		*/
		inline void Blink(uint8_t inIndex) { this->pLights[inIndex].Blink(); }

		/**Turn all the lights on.*/
		void LightOn();
		/**Turn all the lights off.*/
		void LightOff();
		/**Turn all the lights on and blink.*/
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
#ifndef NO_EEPROM
		int EEPROMSave(int inPos, bool inSimulate = false);
		int EEPROMLoad(int inPos);
#endif
#ifdef ACCESSORIES_DEBUG_MODE
		void CheckPort() const;
		void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif

#ifdef ACCESSORIES_PRINT_ACCESSORIES
	public:
		/** Print this accessory on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printAccessory();
		/** Print all the moving positions of this accessory, including blinking mask, on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printMovingPositions();
#endif
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
