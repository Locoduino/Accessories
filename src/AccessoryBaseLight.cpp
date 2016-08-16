/**********************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a light, flashing or not, with optional fading>
***********************************************************************/

#include "Accessories.h"
#include "AccessoryBaseLight.hpp"

#ifndef NO_LIGHT

AccessoryBaseLight::AccessoryBaseLight(Accessory *inpOwner)
{
	this->pPort = NULL;
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

#ifdef DEBUG_MODE
	Serial.print(F("AccessoryBaseLight SetState "));
	Serial.println(inState == LIGHTON ? "ON" : inState == LIGHTOFF ? "OFF" : "BLINK");
#endif

	this->state = inState;
}

void AccessoryBaseLight::SetFading(byte inStep, byte inDelay)
{
	this->fadingStep = inStep;
	this->fadingDelay = inDelay;
	this->fadingCurrentValue = 0;

#ifdef DEBUG_MODE
	if (this->blinkingDelay > 0 && FADING_FULL_DELAY > this->blinkingDelay)
		Serial.println(F("Light fading duration greater than blinking duration !"));
#endif
}

void AccessoryBaseLight::begin(DriverPort *inpPort, int inIntensity, Accessory *inpOwner)
{
	if (inpOwner != 0)
		this->pOwner = inpOwner;
	this->pPort = inpPort;
	this->pPort->SetSpeed(inIntensity);
	this->LightOff();
}

void AccessoryBaseLight::LightFadingRaw(byte inValue)
{
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
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
#ifdef DEBUG_MODE
	Serial.println(F("AccessoryBaseLight ON"));
#endif
	this->state = LIGHTON;
}

void AccessoryBaseLight::LightOff()
{
#ifdef DEBUG_MODE
	Serial.println(F("AccessoryBaseLight OFF"));
#endif
	this->state = LIGHTOFF;
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
	case ACCESSORIES_EVENT_TOGGLE:
		this->Toggle();
		break;

	case ACCESSORIES_EVENT_MOVE:
		switch ((ACCESSORIES_MOVE_TYPE)inData)
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

#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("AccessoryBaseLight start action "));
	Serial.println(this->startingMillis);
#endif
#endif
}

bool AccessoryBaseLight::ActionEnded()
{
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	Serial.println(F("End action of light."));
#endif
#endif
	this->pOwner->ResetAction();

	if (this->state == LIGHTON && this->currentState == LIGHT_ON)
		return true;

	if (this->state == LIGHTOFF && this->currentState == LIGHT_OFF)
		return true;

#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
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
			if (this->state == LIGHTOFF || 
				(this->IsBlinking() && millis() - this->startingMillis > this->blinkingDelay - FADING_FULL_DELAY))
				{
					this->currentState = LIGHT_DESCENDING;
					this->startingMillis = millis();
					this->LightFadingRaw(this->pPort->GetSpeed());
					return false;
				}
			break;
		case LIGHT_OFF:
			if (this->state == LIGHTON ||
				(this->IsBlinking() && millis() - this->startingMillis > this->blinkingDelay - FADING_FULL_DELAY))
			{
				this->currentState = LIGHT_ASCENDING;
				this->startingMillis = millis();
				this->LightFadingRaw(0);
				return false;
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
#endif
