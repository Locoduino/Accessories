/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a motorized accessory>
*************************************************************/

#include "Accessories.h"

#ifndef NO_MOTOR

AccessoryMotor::AccessoryMotor()
{
}

void AccessoryMotor::begin(DriverPort *inpPort, unsigned long inId, int inSpeed, unsigned long inDurationMilli)
{
	this->pPort = inpPort;
	Accessory::begin(STOP);
	this->pPort->SetSpeed(inSpeed);
	this->prevState = STOP;
	this->AddMovingPosition(inId, RIGHT);
}

ACC_STATE AccessoryMotor::MoveToggle(unsigned long inDuration, int inSpeed)
{
	if (this->prevState == LEFT)
		this->InternalMove(RIGHT, inDuration, inSpeed);
	else
		this->InternalMove(LEFT, inDuration, inSpeed);

	return this->state;
}

ACC_STATE AccessoryMotor::InternalMove(ACC_STATE inStateToReach, unsigned long inDuration, int inSpeed)
{
	if (this->IsActionPending())
		return this->GetState();

	unsigned long duration = inDuration == 0 ? this->GetDuration() : inDuration;
	if (duration > 0)
		StartAction();

	if (inStateToReach == RIGHT)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("AccessoryMotor InternalMove() RIGHT"));
#endif
		this->pPort->SetSpeed(inSpeed);
		this->pPort->MoveLeftDir();
		this->state = RIGHT;
	}
	else
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("AccessoryMotor InternalMove() LEFT"));
#endif
		this->pPort->SetSpeed(inSpeed);
		this->pPort->MoveRightDir();
		this->state = LEFT;
	}

	this->prevState = this->state;

	return this->state;
}

void AccessoryMotor::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
{
	switch (inEvent)
	{
	case ACCESSORIES_EVENT_TOGGLE:
		this->MoveToggle();
		break;

	case ACCESSORIES_EVENT_MOVE:
		switch (inData)
		{
		case ACCESSORIES_MOVE_STRAIGHT:
		case ACCESSORIES_MOVE_TOP:
		case ACCESSORIES_MOVE_LEFT:
			this->MoveLeft();
			break;
		case ACCESSORIES_MOVE_DIVERGE:
		case ACCESSORIES_MOVE_BOTTOM:
		case ACCESSORIES_MOVE_RIGHT:
			this->MoveRight();
			break;
		case ACCESSORIES_MOVE_OFF:
		case ACCESSORIES_MOVE_STOP:
			this->pPort->MoveStop();
			this->ResetAction();
			this->state = STOP;
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
		this->SetState((ACC_STATE) inData);
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONID:
		this->SetLastMovingPosition(this->IndexOfMovingPosition(inId));
		this->SetState((ACC_STATE)this->GetMovingPosition(inId));
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONINDEX:
		this->SetLastMovingPosition(inData);
		this->SetState((ACC_STATE)this->GetMovingPositionByIndex(inData));
		break;

	case ACCESSORIES_EVENT_SETSPEED:
		this->pPort->SetSpeed(inData);
		break;

	default:
		break;
	}
}

void AccessoryMotor::SetState(ACC_STATE inState)
{ 
	this->state = inState;

	switch(this->state)
	{
		case LEFT:
			this->StartAction();
			this->pPort->MoveLeftDir();
			break;

		case RIGHT:
			this->StartAction();
			this->pPort->MoveRightDir();
			break;

		default:
			this->pPort->MoveStop();
			this->ResetAction();
	}
}

void AccessoryMotor::Move(unsigned long inId)
{
	if (this->GetMovingPositionSize() == 1)
	{
		this->MoveToggle();
		return;
	}

	int position = this->IndexOfMovingPosition(inId);

	if (position != -1)
		this->SetState((ACC_STATE)position);
}

bool AccessoryMotor::ActionEnded()
{
	bool end = Accessory::ActionEnded();
	if (end)
	{
		this->pPort->MoveStop();
		this->state = STOP;
	}

	return end;
}
#endif
