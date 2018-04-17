/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a stepper motor accessory>
*************************************************************/

// A stepper motor moves only in relative move. So the current position is the last MovingPosition called.

#include "Accessories.h"

#ifndef NO_STEPPER

AccessoryStepper::AccessoryStepper()
{
	this->pPort = NULL;
}

void AccessoryStepper::begin(Port *inpPort, int inStepsNumber, int inReduction, unsigned int inSpeed, int inMovingPositionsNumber)
{
	this->pPort = inpPort;

	((PortStepper *) this->pPort)->SetSpeed(inSpeed);

	Accessory::begin(STATE_NONE);
	this->prevState = STATE_NONE;
	this->stepsNumber = inStepsNumber;
	this->reduction = inReduction;
	this->angleByStep = 360. / inStepsNumber / inReduction;
	this->AdjustMovingPositionsSize(inMovingPositionsNumber);
}

ACC_STATE AccessoryStepper::MoveToggle()
{
	if (this->IsActionDelayPending())
		return this->GetState();

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F("AccessoryStepper MoveToggle() : "));
#endif

	int pos = this->GetLastMovingPosition() + 1;
	if (this->GetLastMovingPosition() == 255)
		pos = 0;
	if (pos >= this->GetMovingPositionSize())
		pos = 0;

	this->Move(this->GetMovingPositionIdByIndex(pos));

	return this->GetState();
}

void AccessoryStepper::Move(unsigned long inId)
{
#ifdef ACCESSORIES_DEBUG_MODE
	Serial.println(F("AccessoryStepper Move()"));
#endif

	if (this->GetMovingPositionSize() == 1)
	{
		this->MoveToggle();
		return;
	}

	this->SetLastMovingPosition(this->IndexOfMovingPositionById(inId));
	this->MovePosition(this->GetMovingPositionValueById(inId));
}

void AccessoryStepper::MovePosition(int inAbsolutePosition)
{
	if (this->IsActionDelayPending())
		return;

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" AccessoryStepper MovePosition() to: "));
	Serial.println(inAbsolutePosition);
#endif

	this->pPort->MovePosition(0, inAbsolutePosition);
}

void AccessoryStepper::MoveRelativePosition(int inRelativePosition)
{
	if (this->IsActionDelayPending())
		return;

#ifdef ACCESSORIES_DEBUG_MODE
	Serial.print(F(" AccessoryStepper MoveRelativePosition() for "));
	Serial.print(inRelativePosition);
	Serial.print(F(" steps."));
#endif

	((PortStepper *)this->pPort)->MoveRelativePosition(0, inRelativePosition);
}

void AccessoryStepper::Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent, int inData)
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
		case ACCESSORIES_MOVE_DIVERGE:
		case ACCESSORIES_MOVE_BOTTOM:
		case ACCESSORIES_MOVE_RIGHT:
		case ACCESSORIES_MOVE_OFF:
		case ACCESSORIES_MOVE_STOP:
			this->pPort->MoveStop();
			this->ResetAction();
			this->SetStateRaw(STATE_NONE);
			break;
		case ACCESSORIES_MOVE_MORE:
		case ACCESSORIES_MOVE_LESS:
			if (this->GetState() == CALIBRATION)
				this->MoveRelativePosition(inData);
			else
			{
				int pos = this->GetLastMovingPosition() + inData;
				if (pos < 0)
					pos = 0;
				if (pos >= this->GetMovingPositionSize())
					pos = this->GetMovingPositionSize() - 1;

				this->Move(this->GetMovingPositionIdByIndex(pos));
			}
			break;
		}
		break;

	case ACCESSORIES_EVENT_MOVEPOSITION:
		this->MovePosition(inData);	// absolute movement
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

	case ACCESSORIES_EVENT_SETDURATION:
		this->SetDuration(inData);
		break;

	default:
		break;
	}
}

bool AccessoryStepper::ActionEnded()
{
	PortStepper *pStepper = (PortStepper *) this->pPort;

	if (pStepper->targetPosition() == pStepper->currentPosition())
		return true;

	pStepper->run();

	if (pStepper->targetPosition() == pStepper->currentPosition())
	{
		this->pPort->MoveStop();
		this->SetStateRaw(STATE_NONE);

#ifdef ACCESSORIES_DEBUG_MODE
		Serial.print(F(" AccessoryStepper end of movement. Pos : "));
		Serial.println(((PortStepper *) this->pPort)->currentPosition());
#endif
	}

	return false;
}

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryStepper::printAccessory()
{
	Serial.print(F("    Stepper : "));
	for (int i = 0; i < this->GetMovingPositionSize(); i++)
	{
		Serial.print(F(" / ID "));
		Serial.print(this->GetMovingPositionIdByIndex(i));
		Serial.print(F(" - pos "));
		Serial.print(this->GetMovingPositionValueByIndex(i));
	}

	Serial.print(F(" / StepsNumber "));
	Serial.print(this->stepsNumber);
	Serial.print(F(" / Reduction "));
	Serial.print(this->reduction);
	if (this->GetPort() != NULL)
	{
		Serial.print(F(" / Speed: "));
		Serial.print(this->GetPort()->GetSpeed());
	}
	Serial.print(F(" / Delay: "));
	Serial.print(this->GetDebounceDelay());
	Serial.print(F(" / "));
	if (this->GetPort() != NULL)
		this->GetPort()->printPort();
	Serial.println(F(" "));
}
#endif
#endif
