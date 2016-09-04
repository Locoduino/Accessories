/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a servo accessory>
*************************************************************/

#include "Accessories.h"

#ifndef NO_SERVO
#include "ActionsStack.hpp"

AccessoryServo::AccessoryServo()
{
	this->pPort = NULL;
	this->minimumPosition = 0;
	this->maximumPosition = 180;
	this->powerState = PowerNoAction;
	this->type = ACCESSORYSERVO;
}

void AccessoryServo::begin(DriverPort *inpPort, unsigned long inDurationMilli, 
	int inMinimumPosition, int inMaximumPosition, int inMovingPositionsNumber)
{
	this->pPort = inpPort;

	this->SetDuration(inDurationMilli);
	Accessory::begin(UNDEFINED);
	this->minimumPosition = inMinimumPosition;
	this->maximumPosition = inMaximumPosition;
	this->prevState = UNDEFINED;
	inpPort->beginByAccessory(this->minimumPosition);
	this->prevState = MINIMUM;
	this->currentPosition = this->minimumPosition;
	this->targetPosition = -1;
	this->targetSpeed = 0;
	this->powerCommandPin = (GPIO_pin_t)DP_INVALID;
	this->powerDelay = 0;
	this->powerState = PowerNoAction;
	this->AdjustMovingPositionsSize(inMovingPositionsNumber);
}

void AccessoryServo::SetPowerCommand(int inPin, unsigned long inDelay)
{
	this->powerCommandPin = Arduino_to_GPIO_pin(inPin);

	pinMode2f(this->powerCommandPin, OUTPUT);
	digitalWrite2f(this->powerCommandPin, HIGH);

	this->powerDelay = inDelay;
}

void AccessoryServo::MoveMinimum()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryServo MoveMinimum()"));
#endif

	this->MovePosition(this->minimumPosition);
}

void AccessoryServo::MoveMaximum()
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryServo MoveMaximum()"));
#endif

	this->MovePosition(this->maximumPosition);
}

ACC_STATE AccessoryServo::MoveToggle()
{
	if (this->IsActionPending())
		return this->GetState();

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryServo MoveToggle() : "));
#endif

	if (this->prevState == MAXIMUM)
		this->MovePosition(this->minimumPosition);
	else
		this->MovePosition(this->maximumPosition);

	return this->state;
}

void AccessoryServo::SetState(ACC_STATE inState) 
{ 
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryServo SetState("));
#endif

	switch (inState)
	{
		case MINIMUM:
#ifdef ACCESSORIES_DEBUG_MODE
			Serial.println(F("MINIMUM)"));
#endif
			this->MovePosition(this->minimumPosition);
			break;

		case MAXIMUM:
#ifdef ACCESSORIES_DEBUG_MODE
			Serial.println(F("MAXIMUM)"));
#endif
			this->MovePosition(this->maximumPosition);
			break;

		default:
			this->ResetAction();
	}
}

void AccessoryServo::Move(unsigned long inId)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryServo Move()"));
#endif

	if (this->GetMovingPositionSize() == 1)
	{
		this->MoveToggle();
		return;
	}

	this->SetLastMovingPosition(this->IndexOfMovingPosition(inId));
	int position = this->GetMovingPosition(inId);

	if (position == MINIMUM || position == MAXIMUM)
		this->SetState((ACC_STATE)position);
	else
		this->MovePosition(position);
}

void AccessoryServo::MovePosition(int inPosition)
{
	if (this->IsActionPending() && !ActionsStack::FillingStack)
		return;

	this->InternalMovePosition(inPosition);
}

void AccessoryServo::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	switch (inEvent)
	{
	case ACCESSORIES_EVENT_TOGGLE:
		this->Move(inId);
		break;

	case ACCESSORIES_EVENT_MOVE:
		switch (inData)
		{
		case ACCESSORIES_MOVE_STRAIGHT:
		case ACCESSORIES_MOVE_TOP:
		case ACCESSORIES_MOVE_LEFT:
			this->MoveMinimum();
			break;
		case ACCESSORIES_MOVE_DIVERGE:
		case ACCESSORIES_MOVE_BOTTOM:
		case ACCESSORIES_MOVE_RIGHT:
			this->MoveMaximum();
			break;
		case ACCESSORIES_MOVE_OFF:
		case ACCESSORIES_MOVE_STOP:
			this->pPort->MoveStop();
			this->ResetAction();
			this->state = STOP;
			break;
		case ACCESSORIES_MOVE_MORE:
		case ACCESSORIES_MOVE_LESS:
			{
			int pos = this->currentPosition + inData;
			if (pos > this->maximumPosition)
				pos = this->maximumPosition;
			if (pos < this->minimumPosition)
				pos = this->minimumPosition;
			this->MovePosition(pos);
			}
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
		this->MovePosition(inData);
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONID:
		this->Move(inId);
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONINDEX:
		this->Move(this->GetMovingPositionIdByIndex(inData));
		break;

	case ACCESSORIES_EVENT_SETSPEED:
		this->pPort->SetSpeed(inData);
		break;

	default:
		break;
	}
}

void AccessoryServo::InternalMovePosition(int inPosition)
{
	if (this->currentPosition == inPosition)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("AccessoryServo::MovePosition : same position - no move!"));
#endif
		return;
	}

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryServo InternalMovePosition()"));
#endif

	MovementSpeed speed = this->GetMovementSpeed();

	switch (speed)
	{
	case ServoFast:
		this->pPort->MovePosition(this->GetDuration(), inPosition);
		this->currentPosition = inPosition;
		break;

	case ServoAlone:
		for (int pos = this->currentPosition; pos < inPosition; pos++)
			this->pPort->MovePosition(this->GetDuration(), pos);
		this->currentPosition = inPosition;
		break;

	case ServoActionStack:
		StartAction();
		this->targetPosition = inPosition;
		this->targetSpeed = (this->currentPosition < this->targetPosition) ? 1 : -1;
		ActionsStack::FillingStack = true;
		break;

	case ServoSlow:
		StartAction();
		this->targetPosition = inPosition;
		this->targetSpeed = (this->currentPosition < this->targetPosition) ? 1 : -1;
		break;
	}

	if (inPosition == this->minimumPosition)
	{
		this->state = MINIMUM;
		this->prevState = MINIMUM;
	}
	else
		if (inPosition == this->maximumPosition)
		{
		this->state = MAXIMUM;
		this->prevState = MAXIMUM;
		}
		else
		{
			this->state = UNDEFINED;
			this->prevState = UNDEFINED;
		}
}

bool AccessoryServo::ActionEnded()
{
	if (this->GetActionStartingMillis() <= 0)
		return true;

	if (this->powerCommandPin == DP_INVALID || this->powerState == PowerRunning)
	{
		//if (millis() - this->GetActionStartingMillis() > this->GetDuration())
		{
			if (this->targetSpeed == 0)
			{
				// Abort
				this->targetPosition = -1;
				ActionsStack::FillingStack = false;
				if (this->powerState == PowerRunning && this->powerCommandPin != DP_INVALID)
				{
#ifdef ACCESSORIES_DEBUG_MODE
					Serial.println(F("AccessoryServo after running"));
#endif
					this->powerState = PowerAfterRunning;
					StartAction();
					return false;
				}

				this->ResetAction();
				return true;
			}

			this->currentPosition += this->targetSpeed;
			this->pPort->MovePosition(this->GetDuration(), this->currentPosition);

			if (this->targetPosition == this->currentPosition)
			{
				this->targetPosition = -1;
				this->targetSpeed = 0;
				ActionsStack::FillingStack = false;
				if (this->powerState == PowerRunning && this->powerCommandPin != DP_INVALID)
				{
#ifdef ACCESSORIES_DEBUG_MODE
					Serial.println(F("AccessoryServo after running"));
#endif
					this->powerState = PowerAfterRunning;
					StartAction();
					return false;
				}
				this->ResetAction();
				return true;
			}
			StartAction();
		}
	}
	else
	{
		if (this->powerState == PowerNoAction)
		{
#ifdef ACCESSORIES_DEBUG_MODE
			Serial.println(F("AccessoryServo start power command"));
#endif
			digitalWrite2f(this->powerCommandPin, LOW);
			this->powerState = PowerBeforeRunning;
			StartAction();
			return false;
		}

		// Action time elapsed, do the next work...
		if (millis() - this->GetActionStartingMillis() > this->powerDelay)
		{
			if (this->powerState == PowerBeforeRunning)
			{
#ifdef ACCESSORIES_DEBUG_MODE
				Serial.println(F("AccessoryServo running"));
#endif
				this->powerState = PowerRunning;
				StartAction();
				return false;
			}

			if (this->powerState == PowerAfterRunning)
			{
#ifdef ACCESSORIES_DEBUG_MODE
				Serial.println(F("AccessoryServo end running"));
#endif
				this->powerState = PowerNoAction;
				digitalWrite2f(this->powerCommandPin, HIGH);
				this->ResetAction();
				return true;
			}
		}
	}

	return false;
}

MovementSpeed AccessoryServo::GetMovementSpeed() const
{
	if (this->GetDuration() == 0)
		return ServoFast;
	if (this->GetDuration() < 6)
		return ServoAlone;
	if (this->GetDuration() < 20)
		return ServoActionStack;
	return ServoSlow;
}

#endif
