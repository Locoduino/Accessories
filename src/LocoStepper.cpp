#include "Accessories.h"

#ifndef NO_STEPPER
#include "LocoStepper.h"

uint8_t step2default[4] =
{
	B01,
	B11,
	B10,
	B00,
};

uint8_t step4default[4] =
{
	B1010,
	B0110,
	B0101,
	B1001,
};
LocoStepper::LocoStepper(uint8_t pin1, uint8_t pin2, uint8_t *pStep2) : LocoStepper(pin1, pin2, 255, 255)
{
	if (pStep2 == NULL)
		this->pSteps = step2default;
	else
		this->pSteps = pStep2;
}

LocoStepper::LocoStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t *pStep4)
{
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _stepInterval = 0;
    _lastRunTime = 0;
    _minPulseWidth = 1;
    _lastStepTime = 0;
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    _pin4 = pin4;
	if (pStep4 == NULL)
		this->pSteps = step4default;
	else
		this->pSteps = pStep4;
	enableOutputs();
}

void LocoStepper::moveTo(long absolute)
{
    _targetPos = absolute;

	long distanceTo = distanceToGo();

	if (distanceTo == 0)
		setSpeed(0.0f); // We're there
	if (distanceTo > 0) // Clockwise
		setSpeed((float) fabs(_speed));
	else
		setSpeed((float) -fabs(_speed));
}

void LocoStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if we are still running to position
boolean LocoStepper::run()
{
	if (_targetPos == _currentPos)
		return false;

	unsigned long time = micros();

	if ((time >= (_lastStepTime + _stepInterval))  // okay if both current time and next step time wrap
		|| ((time < _lastRunTime) && (time >(0xFFFFFFFF - (_lastStepTime + _stepInterval)))))  // check if only current time has wrapped
	{
		if (_speed > 0.0f)
		{
			// Clockwise
			_currentPos += 1;
		}
		else 
			if (_speed < 0.0f)
			{
				// Anticlockwise  
				_currentPos -= 1;
			}

		if (this->_pin3 == 255)
			step2(_currentPos & 0x3); // Bottom 2 bits (same as mod 4, but works with + and - numbers) 
		else
			step4(_currentPos & 0x3); // Bottom 2 bits (same as mod 4, but works with + and - numbers) 

		_lastRunTime = time;
		_lastStepTime = time;
	}
	else
		_lastRunTime = time;

	return true;
}

long LocoStepper::distanceToGo()
{
    return _targetPos - _currentPos;
}

long LocoStepper::targetPosition()
{
    return _targetPos;
}

long LocoStepper::currentPosition()
{
    return _currentPos;
}

// Useful during initializations or after initial positioning
void LocoStepper::setCurrentPosition(long position)
{
    _currentPos = position;
}

void LocoStepper::setSpeed(float speed)
{
	_speed = speed;
	_stepInterval = (unsigned long) fabs(1000000.0 / _speed);
}

float LocoStepper::speed()
{
    return _speed;
}

// 2 pin step function
// This is passed the current step number (0 to 3)
// Subclasses can override
void LocoStepper::step2(uint8_t step)
{
    if ((this->pSteps[step] & B10) > 0)
        digitalWrite(_pin1, HIGH);
    else
        digitalWrite(_pin1, LOW);
        
    if ((this->pSteps[step] & B01) > 0)
        digitalWrite(_pin2, HIGH);
    else
        digitalWrite(_pin2, LOW);
}

// 4 pin step function
// This is passed the current step number (0 to 3)
// Subclasses can override
void LocoStepper::step4(uint8_t step)
{
    if ((this->pSteps[step] & B1000) > 0)
        digitalWrite(_pin1, HIGH);
    else
        digitalWrite(_pin2, LOW);
    if ((this->pSteps[step] & B0100) > 0)
        digitalWrite(_pin2, HIGH);
    else
        digitalWrite(_pin3, LOW);
    if ((this->pSteps[step] & B0010) > 0)
        digitalWrite(_pin3, HIGH);
    else
        digitalWrite(_pin4, LOW);
    if ((this->pSteps[step] & B0001) > 0)
        digitalWrite(_pin4, HIGH);
    else
        digitalWrite(_pin1, LOW);
}


// Prevents power consumption on the outputs
void    LocoStepper::disableOutputs()
{   
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
    if (_pin3 != 255)
	   digitalWrite(_pin3, LOW);
    if (_pin4 != 255)
       digitalWrite(_pin4, LOW);
}

void    LocoStepper::enableOutputs()
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    if (_pin3 != 255)
    	pinMode(_pin3, OUTPUT);
    if (_pin4 != 255)
	   pinMode(_pin4, OUTPUT);
}

void LocoStepper::setMinPulseWidth(unsigned int minWidth)
{
  _minPulseWidth = minWidth;
}
#endif
