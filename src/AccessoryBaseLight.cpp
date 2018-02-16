/**********************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a light, flashing or not, with optional fading>
***********************************************************************/

#include "Accessories.h"
#include "AccessoryBaseLight.hpp"
#ifndef NO_EEPROM
#include "EEPROM.h"
#endif

#ifndef NO_LIGHT

AccessoryBaseLight::AccessoryBaseLight(Accessory *inpOwner)
{
	this->pPort = NULL;
	this->state = LIGHTOFF;
	this->currentState = LIGHT_OFF;
	this->startingMillis = 0;
	this->fadingStep = this->fadingDelay = 0;
	this->blinkingDelay = 0;
	this->pOwner = inpOwner;
}

void AccessoryBaseLight::SetState(ACC_STATE inState)
{ 
	if (inState == LIGHTBLINK && this->blinkingDelay == 0)
		inState = LIGHTON;

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryBaseLight SetState "));
	Serial.println(inState == LIGHTON ? "ON" : inState == LIGHTOFF ? "OFF" : "BLINK");
#endif

	this->SetStateRaw(inState);
}

void AccessoryBaseLight::SetStateRaw(ACC_STATE inNewState)
{
	if (this->state != inNewState)
	{
		this->state = inNewState;
		this->pOwner->SetStateRaw(inNewState);
	}
}

void AccessoryBaseLight::SetFading(uint8_t inStep, uint8_t inDelay)
{
	this->fadingStep = inStep;
	this->fadingDelay = inDelay;
	this->fadingCurrentValue = 0;

#ifdef ACCESSORIES_DEBUG_MODE
	if (this->pPort->GetPinType() == DIGITAL || this->pPort->GetPinType() == DIGITAL_INVERTED)
		Serial.println(F("Fading on a light is not allowed on a DIGITAL pin !"));
	if (this->blinkingDelay > 0 && FADING_FULL_DELAY > this->blinkingDelay)
		Serial.println(F("Light fading duration greater than blinking duration !"));
#endif
}

void AccessoryBaseLight::begin(Port *inpPort, int inIntensity, Accessory *inpOwner)
{
	if (inpOwner != NULL)
		this->pOwner = inpOwner;
	this->pPort = inpPort;
	this->pPort->SetSpeed(inIntensity);
	this->LightOff();
}

void AccessoryBaseLight::LightFadingRaw(uint8_t inValue)
{
#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F("AccessoryBaseLight Fading at "));
	Serial.println(inValue);
#endif
#endif
	this->fadingCurrentValue = inValue;
	this->pPort->MoveLeftDir(-1, inValue);
}

void AccessoryBaseLight::LightOnRaw()
{
	this->LightFadingRaw(this->pPort->GetSpeed());
}

void AccessoryBaseLight::LightOffRaw()
{
	this->LightFadingRaw(0);
	this->pPort->MoveStop();
}

void AccessoryBaseLight::LightOn()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryBaseLight ON"));
#endif
	this->SetState(LIGHTON);
}

void AccessoryBaseLight::LightOff()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryBaseLight OFF"));
#endif
	this->SetState(LIGHTOFF);
}

ACC_STATE AccessoryBaseLight::Toggle()
{
	if (this->state == LIGHTON || this->state == LIGHTBLINK)
		this->SetState(LIGHTOFF);
	else
		this->SetState(LIGHTBLINK);	// will be converted in LIGHTON if Duration is 0 !

	return this->state; 
}

void AccessoryBaseLight::Event(ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	switch (inEvent)
	{
	case ACCESSORIES_EVENT_MOVEPOSITIONID:
	case ACCESSORIES_EVENT_TOGGLE:
		this->Toggle();
		break;

	case ACCESSORIES_EVENT_MOVE:
		switch (inData)
		{
		case ACCESSORIES_MOVE_STRAIGHT:
		case ACCESSORIES_MOVE_TOP:
		case ACCESSORIES_MOVE_LEFT:
		case ACCESSORIES_MOVE_DIVERGE:
		case ACCESSORIES_MOVE_BOTTOM:
		case ACCESSORIES_MOVE_RIGHT:
		case ACCESSORIES_MOVE_ON:
			this->LightOn();
			break;
		case ACCESSORIES_MOVE_OFF:
		case ACCESSORIES_MOVE_STOP:
			this->LightOff();
			break;
		case ACCESSORIES_MOVE_MORE:
		case ACCESSORIES_MOVE_LESS:
			{
			int oldValue = this->pPort->GetSpeed();
			this->pPort->SetSpeed(oldValue + inData);
			this->LightOn();
			this->pPort->SetSpeed(oldValue);
			}
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
		{
			int oldValue = this->pPort->GetSpeed();
			this->pPort->SetSpeed(inData);
			this->LightOn();
			this->pPort->SetSpeed(oldValue);
		}
		break;

	case ACCESSORIES_EVENT_SETSPEED:
		this->pPort->SetSpeed(inData);
		break;

	default:
		break;
	}
}

void AccessoryBaseLight::StartAction()
{
	if (this->blinkingDelay > 0 || this->IsFading())
	{
		this->startingMillis = millis();
	}

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F("AccessoryBaseLight start action "));
	Serial.println(this->startingMillis);
#endif
#endif
}

bool AccessoryBaseLight::ActionEnded()
{
#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	if (this->pOwner->IsActionDelayPending())
		Serial.println(F("End action of light."));
#endif
#endif
	this->pOwner->ResetAction();

	if (this->state == LIGHTON && this->currentState == LIGHT_ON)
		return true;

	if (this->state == LIGHTOFF && this->currentState == LIGHT_OFF)
		return true;

#ifdef ACCESSORIES_DEBUG_MODE
#ifdef ACCESSORIES_DEBUG_VERBOSE_MODE
	Serial.print(F("Light current state : "));
	switch (this->currentState)
	{
	case LIGHT_ON:
		Serial.println(F("On"));
		break;
	case LIGHT_OFF:
		Serial.println(F("Off"));
		break;
	case LIGHT_ASCENDING:
		Serial.println(F("Ascending"));
		break;
	case LIGHT_DESCENDING:
		Serial.println(F("Descending"));
		break;
	}
#endif
#endif
	switch (this->currentState)
	{
		case LIGHT_ON:
			if (this->state == LIGHTOFF || this->IsBlinking())
			{
				if (this->IsFading() || (this->IsBlinking() && millis() - this->startingMillis > this->blinkingDelay - FADING_FULL_DELAY))
				{
					this->currentState = LIGHT_DESCENDING;
					this->startingMillis = millis();
					this->LightFadingRaw(this->pPort->GetSpeed());
					return false;
				}
				else if (!this->IsBlinking())
				{
					this->currentState = LIGHT_OFF;
					this->pPort->MoveStop();
				}
			}
			break;
		case LIGHT_OFF:
			if (this->state == LIGHTON || this->IsBlinking())
			{
				if (this->IsFading() || (this->IsBlinking() && millis() - this->startingMillis > this->blinkingDelay - FADING_FULL_DELAY))
				{
					this->currentState = LIGHT_ASCENDING;
					this->startingMillis = millis();
					this->LightFadingRaw(0);
					return false;
				}
				else if (!this->IsBlinking())
				{
					this->currentState = LIGHT_ON;
					this->pPort->MoveLeftDir();
				}
			}
			break;
		case LIGHT_ASCENDING:
		{
			bool endStateAsc = false;
			if (this->fadingStep == 0)
				endStateAsc = true;
			else
				if (millis() - this->startingMillis > this->fadingDelay)
				{
					int curr = (int) this->fadingCurrentValue + this->fadingStep;
					if (curr > this->pPort->GetSpeed() || curr < 0)
						endStateAsc = true;
					else
					{
						this->LightFadingRaw((unsigned char)curr);
						this->startingMillis = millis();
					}
				}
				//else
					//Serial.println("Fading delay not finished");
			if (endStateAsc)
			{
				// End of fading
				this->currentState = LIGHT_ON;
				if (this->IsBlinking())
					this->startingMillis = millis();
				else
					this->startingMillis = 0;
				this->LightOnRaw();
				return true;
			}
		}
			break;

		case LIGHT_DESCENDING:
		{
			bool endStateDesc = false;
			if (this->fadingStep == 0)
				endStateDesc = true;
			else
				if (millis() - this->startingMillis > this->fadingDelay)
				{
					int curr = (int) this->fadingCurrentValue - this->fadingStep;
					if (curr <= 0)
						endStateDesc = true;
					else
					{
						this->LightFadingRaw((unsigned char)curr);
						this->startingMillis = millis();
					}
				}
				//else
					//Serial.println("Fading delay not finished");
			if (endStateDesc)
			{
				// End of fading
				this->currentState = LIGHT_OFF;
				if (this->IsBlinking())
					this->startingMillis = millis();
				else
					this->startingMillis = 0;
				this->LightOffRaw();
				return true;
			}
		}
			break;

	}
	return false;
}

#ifndef NO_EEPROM
int AccessoryBaseLight::EEPROMSave(int inPos, bool inSimulate)
{
	if (!inSimulate)
		EEPROM.write(inPos, this->state);

	return inPos + 1;
}

int AccessoryBaseLight::EEPROMLoad(int inPos)
{
	this->state = (ACC_STATE)EEPROM.read(inPos++);

	return inPos;
}
#endif
#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryBaseLight::printAccessory()
{
	Serial.print(F("Fading Step: "));
	Serial.print(this->fadingStep);
	Serial.print(F(" / Fading Delay: "));
	Serial.print(this->fadingDelay);
	Serial.print(F(" / Blinking Delay: "));
	Serial.print(this->blinkingDelay);
	Serial.print(F(" / "));
	if (this->GetPort() != NULL)
		this->GetPort()->printPort();
	Serial.println(F(" "));
}
#endif

#endif
