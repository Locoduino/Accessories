// LocoStepper.h
//
/// \file LocoStepper.h
///
/// This is the Arduino LocoStepper class.
/// It provides an object-oriented interface for 2 or 4 pin stepper motors.
///
/// The standard Arduino IDE includes the Stepper library
/// (http://arduino.cc/en/Reference/Stepper) for stepper motors. It is
/// perfectly adequate for simple, single motor applications.
///
/// LocoStepper significantly improves on the standard Arduino Stepper library in several ways:
/// \li Supports multiple simultaneous steppers, with independent concurrent stepping on each stepper
/// \li API functions never delay() or block
/// \li Supports 2 and 4 wire steppers
/// \li Very slow speeds are supported
///
/// Example Arduino programs are included to show the main modes of use.
///
/// This software is Copyright (C) 2016 thierry Paris / Locoduino. Use is subject to license
/// conditions. The main licensing options available are GPL V2 or Commercial:
/// 
/// \par Open Source Licensing GPL V2
/// This is the appropriate option if you want to share the source code of your
/// application with everyone you distribute it to, and you also want to give them
/// the right to share who uses it. If you wish to use this software under Open
/// Source Licensing, you must contribute all your source code to the open source
/// community in accordance with the GPL Version 2 when your application is
/// distributed. See http://www.gnu.org/copyleft/gpl.html
/// 
/// \par Commercial Licensing
/// This is the appropriate option if you are creating proprietary applications
/// and you are not prepared to distribute and share the source code of your
/// application. Contact info@open.com.au for details.
///
/// \par Revision History
/// \version 1.0 Initial release. This is a massively reduced version of AccelStepper library 
/// (http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.57.zip) from Mike McCauley.
///	which was too much powerful, and seems to have problems to correctly return to the same position.

#ifndef NO_STEPPER

#ifndef LocoStepper_h
#define LocoStepper_h

/////////////////////////////////////////////////////////////////////
/// \class LocoStepper LocoStepper.h <LocoStepper.h>
/// \brief Support for stepper motors with acceleration etc.
///
/// This defines a single 2 or 4 pin stepper motor, or stepper motor with fdriver chip, with optional
/// absolute positioning commands etc. Multiple	simultaneous steppers are supported, all moving 
/// at different speeds. 
///
/// \par Operation
/// This module operates by computing a step time in microseconds. The step
/// time is recomputed after each step and after speed and acceleration
/// parameters are changed by the caller. The time of each step is recorded in
/// microseconds. The run() function steps the motor if a new step is due.
/// The run() function must be called frequently until the motor is in the
/// desired position, after which time run() will do nothing.
///
/// \par Positioning
/// Positions are specified by a signed long integer. At
/// construction time, the current position of the motor is consider to be 0. Positive
/// positions are clockwise from the initial position; negative positions are
/// anticlockwise. The current position can be altered for instance after
/// initialization positioning.
///
/// \par Caveats
/// This is an open loop controller: If the motor stalls or is over-speed,
/// LocoStepper will not have a correct 
/// idea of where the motor really is (since there is no feedback of the motor's
/// real position. We only know where we _think_ it is, relative to the
/// initial starting point).
///
/// The fastest motor speed that can be reliably supported is 4000 steps per
/// second (4 kHz) at a clock frequency of 16 MHz. However, any speed less than that
/// down to very slow speeds (much less than one per second) are also supported,
/// provided the run() function is called frequently enough to step the motor
/// whenever required for the speed set.
class LocoStepper
{
public:
    /// Constructor. You can have multiple simultaneous steppers, all moving
    /// at different speeds and accelerations, provided you call their run()
    /// functions at frequent enough intervals. Current Position is set to 0, target
    /// position is set to 0.
    /// The motor pins will be initialized to OUTPUT mode during the
    /// constructor by a call to enableOutputs().
	/// inpSteps pointer argument gives the caller the possibility to fix himself the sequence
	/// of pins activation. If nothing is specified, a default value will be used:
	/// \verbatim
	///	B01, B11, B10, B00 
	/// \endverbatim
    LocoStepper(uint8_t pin1, uint8_t pin2, uint8_t *inpStep2 = NULL);
	/// Constructor. You can have multiple simultaneous steppers, all moving
	/// at different speeds and accelerations, provided you call their run()
	/// functions at frequent enough intervals. Current Position is set to 0, target
	/// position is set to 0.
	/// The motor pins will be initialized to OUTPUT mode during the
	/// constructor by a call to enableOutputs().
	/// inpSteps pointer argument gives the caller the possibility to fix himself the sequence
	/// of pins activation. If nothing is specified, a default value will be used:
	/// \verbatim
	/// B1010, B0110, B0101, B1001
	/// \endverbatim
	LocoStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t *inpStep2 = NULL);

    /// Set the target position. The run() function will try to move the motor
    /// from the current position to the target position set by the most
    /// recent call to this function.
    /// \param[in] absolute The desired absolute position. Negative is
    /// anticlockwise from the 0 position.
    void    moveTo(long absolute); 

    /// Set the target position relative to the current position
    /// \param[in] relative The desired position relative to the current position. Negative is
    /// anticlockwise from the current position.
    void    move(long relative);

    /// Poll the motor and step it if a step is due. You must call this as
    /// frequently as possible, but at least once per minimum step interval,
    /// preferably in your main loop.
    /// \return true if the motor is still going to the target position.
    boolean run();

    /// Sets the desired constant speed for use with run().
    /// \param[in] speed The desired constant speed in steps per
    /// second. Positive is clockwise. Speeds of more than 1000 steps per
    /// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
    /// once per hour, approximately. Speed accuracy depends on the Arduino
    /// crystal. Jitter depends on how frequently you call the runSpeed() function.
    void    setSpeed(float speed);

    /// The most recently set speed
    /// \return the most recent speed in steps per second
    float   speed();

    /// The distance from the current position to the target position.
    /// \return the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    long    distanceToGo();

    /// The most recently set target position.
    /// \return the target position
    /// in steps. Positive is clockwise from the 0 position.
    long    targetPosition();

    /// The currently motor position.
    /// \return the current motor position
    /// in steps. Positive is clockwise from the 0 position.
    long    currentPosition();  

    /// Resets the current position of the motor, so that wherever the motor
    /// happens to be right now is considered to be the new position. Useful
    /// for setting a zero position on a stepper after an initial hardware
    /// positioning move.
    /// \param[in] position The position in steps of wherever the motor
    /// happens to be right now.
    void    setCurrentPosition(long position);  
    
    /// Disable motor pin outputs by setting them all LOW
    /// Depending on the design of your electronics this may turn off
    /// the power to the motor coils, saving power.
    /// This is useful to support Arduino low power modes: disable the outputs
    /// during sleep and then re-enable with enableOutputs() before stepping
    /// again.
    void    disableOutputs();

    /// Enable motor pin outputs by setting the motor pins to OUTPUT
    /// mode. Called automatically by the constructor.
    void    enableOutputs();

    /// Sets the minimum pulse width allowed by the stepper driver.
    /// \param[in] minWidth The minimum pulse width in microseconds.
    void    setMinPulseWidth(unsigned int minWidth);

protected:

    /// Called to execute a step. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default calls step1(), step2() or step4() depending on the
    /// number of pins defined for the stepper.
    /// \param[in] step The current step phase number (0 to 3)
    void   step(uint8_t step);

    /// Called to execute a step on a 2 pin motor. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1 and pin2
    /// \param[in] step The current step phase number (0 to 3)
    void   step2(uint8_t step);

    /// Called to execute a step on a 4 pin motor. Only called when a new step is
    /// required. Subclasses may override to implement new stepping
    /// interfaces. The default sets or clears the outputs of pin1, pin2,
    /// pin3, pin4.
    /// \param[in] step The current step phase number (0 to 3)
    void   step4(uint8_t step);

private:
    /// Arduino pin number for the 2 or 4 pins required to interface to the
    /// stepper motor.
	/// For two wire motor, pin3 and pin4 have 255 value.
    uint8_t _pin1, _pin2, _pin3, _pin4;

    uint8_t *pSteps;
	bool invertedDigital;

    /// The current absolution position in steps.
    long           _currentPos;    // Steps

    /// The target position in steps. The LocoStepper library will move the
    /// motor from the _currentPos to the _targetPos, taking into account the
    /// max speed, acceleration and deceleration
    long           _targetPos;     // Steps

    /// The current motor speed in steps per second
    /// Positive is clockwise
    float          _speed;         // Steps per second

    /// The current interval between steps in microseconds
    unsigned long  _stepInterval;
	   
    /// The last run time (when runSpeed() was last called) in microseconds
    unsigned long  _lastRunTime;

    /// The last step time in microseconds
    unsigned long  _lastStepTime;

    /// The minimum allowed pulse width in microseconds
    unsigned int   _minPulseWidth;
};

#endif 
#endif 
