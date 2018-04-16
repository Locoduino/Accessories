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

void AccessoryMotor::begin(Port *inpPort, unsigned long inId, int inSpeed, unsigned long inDurationMilli)
{
	this->pPort = inpPort;
	Accessory::begin(STOP);
	this->pPort->SetSpeed(inSpeed);
	this->SetDuration(inDurationMilli);
	this->prevState = STOP;
	this->AddMovingPosition(inId, RIGHT);
}

ACC_STATE AccessoryMotor::MoveToggle(unsigned long inDuration, int inSpeed)
{
	if (this->prevState == LEFT)
		this->InternalMove(RIGHT, inDuration, inSpeed);
	else
		this->InternalMove(LEFT, inDuration, inSpeed);

	return this->GetState();
}

ACC_STATE AccessoryMotor::InternalMove(ACC_STATE inStateToReach, unsigned long inDuration, int inSpeed)
{
	if (this->IsActionDelayPending())
		return this->GetState();

	unsigned long duration = inDuration == 0 ? this->GetDuration() : inDuration;
	if (duration > 0)
		StartAction();

	if (inSpeed > 0)
		this->pPort->SetSpeed(inSpeed);
	if (inStateToReach == RIGHT)
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("AccessoryMotor InternalMove() RIGHT"));
#endif
		this->pPort->MoveLeftDir();
		this->SetStateRaw(RIGHT);
	}
	else
	{
#ifdef ACCESSORIES_DEBUG_MODE
		Serial.println(F("AccessoryMotor InternalMove() LEFT"));
#endif
		this->pPort->MoveRightDir();
		this->SetStateRaw(LEFT);
	}

	this->prevState = this->GetState();

	return this->GetState();
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
			this->MoveStop();
			this->ResetAction();
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
		this->SetState((ACC_STATE) inData);
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONID:
		this->SetLastMovingPosition(this->IndexOfMovingPositionById(inId));
		this->SetState((ACC_STATE)this->GetMovingPositionValueById(inId));
		break;

	case ACCESSORIES_EVENT_MOVEPOSITIONINDEX:
		this->SetLastMovingPosition(inData);
		this->SetState((ACC_STATE)this->GetMovingPositionValueByIndex(inData));
		break;

	case ACCESSORIES_EVENT_SETSPEED:
		this->pPort->SetSpeed(inData);
		break;

	case ACCESSORIES_EVENT_SETDURATION:
		this->SetDuration(inData);
		break;

	case ACCESSORIES_EVENT_EXTERNALMOVE:
		this->ExternalMove((ACC_STATE)inData);
		break;

	default:
		break;
	}
}

void AccessoryMotor::SetState(ACC_STATE inState)
{ 
	this->SetStateRaw(inState);

	switch(this->GetState())
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

	int position = this->IndexOfMovingPositionById(inId);

	if (position != -1)
		this->SetState((ACC_STATE)position);
}

void AccessoryMotor::MoveStop()
{
	this->pPort->MoveStop();
	this->SetStateRaw(STOP);
}

bool AccessoryMotor::ActionEnded()
{
	bool end = Accessory::ActionEnded();
	if (end)
	{
		this->MoveStop();
	}

	return end;
}

#ifndef NO_EEPROM
int AccessoryMotor::EEPROMLoad(int inPos)
{
	inPos = this->Accessory::EEPROMLoad(inPos);

	switch (this->GetState())
	{
	case LEFT:	this->pPort->MoveLeftDir(); break;
	case RIGHT:	this->pPort->MoveRightDir(); break;
	case STOP:	this->pPort->MoveStop(); break;
	}

	return inPos;
}
#endif
#endif
