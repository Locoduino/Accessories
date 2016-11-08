////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
#define ACCESSORIES_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if ACCESSORIES_DEBUG_MODE is not activated.
#define ACCESSORIES_DEBUG_VERBOSE_MODE

#define  GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#define ACCESSORIES_DEBUG_MODE
#include "../../DIO2/VStudio/DIO2.h"
#else
#include "DIO2.h"
#endif


#define ACTION_STACK_SIZE	5

//////////////////////////////////////////
//  Exclusion area
//
// You can exclude some parts of the library here, to avoid losing program and data memory
// on parts you dont use.
// For example, if you dont want servos
// just uncomment the line #define NO_SERVO by removing // at the beggining.
//
// Arduino IDE build of list of the files to compile from the content of the library directory
// in a makefile and give it to Gcc. So Gcc compiles all the files of the directory
// even if its content is not used. The result is, if an unused source file contains
// static declarations, these statics will be allocated a this memory will be lost.
// The only solution I have found is to rename the source files to something IDE and Gcc dont know...
//
// So if you want to lose less memory, you can rename the linked files
// from .cpp to .ori, and from .hpp to from ori.
// See below the file names related to each exclusion:
//
//NO_GROUP
//	AccessoryGroup.cpp
//	AccessoryGroup.hpp
//
//NO_MOTOR
//	AccessoryMotor.cpp
//	AccessoryMotor.hpp
//	AccessoryMotorOneWay.cpp
//	AccessoryMotorOneWay.hpp
//	AccessoryMotorTwoWays.cpp
//	AccessoryMotorTwoWays.hpp
//	AFMotor.cpp
//	AFMotor.hpp
//
//NO_LIGHT
//	AccessoryBaseLight.cpp
//	AccessoryBaseLight.hpp
//	AccessoryLight.cpp
//	AccessoryLight.hpp
//	AccessoryLightMulti.cpp
//	AccessoryLightMulti.hpp
//
//NO_MOTOR_LIGHT
//	PortOnePin.cpp
//	PortOnePin.hpp
//	PortTwoPins.cpp
//	PortTwoPins.hpp
//	PortTwoPinsEnable.cpp
//	PortTwoPinsEnable.hpp
//	PortSpeedDirBrake.cpp
//	PortSpeedDirBrake.hpp
//	
//NO_SERVO
//	AccessoryServo.cpp
//	AccessoryServo.hpp
//	PortServo.cpp
//	PortServo.hpp
//	Servo.cpp
//	Servo.hpp
//
//NO_SHIELDL293D
//	PortShieldL293d.cpp
//	PortShieldL293d.hpp
//
//NO_STEPPER
//	AccessoryStepper.cpp
//	AccessoryStepper.hpp
//	DriverStepper.cpp
//	DriverStepper.hpp
//	PortStepper.cpp
//	PortStepper.hpp
//

//#define NO_GROUP
//#define NO_MOTOR
//#define NO_SERVO
//#define NO_STEPPER
//#define NO_LIGHT
//#define NO_SHIELDL293D

//#define NO_EEPROM

#ifdef NO_MOTOR
	#ifdef NO_LIGHT
		#define NO_MOTOR_LIGHT
	#endif
	#ifndef NO_MOTORONEWAY
		#define NO_MOTORONEWAY
	#endif
	#ifndef NO_MOTORTWOWAYS
		#define NO_MOTORTWOWAYS
	#endif
#endif

// For Accessories library, L293D is not compatible with Arduino Due for the moment...
#ifdef ARDUINO_ARCH_SAM
#ifndef NO_SHIELDL293D
	#define NO_SHIELDL293D
#endif
#endif

/////////////////////////////////////

#include "Port.hpp"

#ifndef NO_MOTOR
#include "AccessoryMotorOneWay.hpp"
#include "AccessoryMotorTwoWays.hpp"
#endif
#ifndef NO_SERVO
#include "AccessoryServo.hpp"
#endif
#ifndef NO_STEPPER
#include "AccessoryStepper.hpp"
#endif
#ifndef NO_LIGHT
#include "AccessoryLight.hpp"
#include "AccessoryLightMulti.hpp"
#endif

#ifndef NO_GROUP
#include "AccessoryGroup.hpp"
#endif

#ifndef NO_MOTOR_LIGHT
#include "PortOnePin.hpp"
#include "PortTwoPins.hpp"
#include "PortTwoPinsEnable.hpp"
#include "PortSpeedDirBrake.hpp"
#endif

#ifndef NO_SHIELDL293D
#include "PortShieldL293d.hpp"
#endif

#ifndef NO_SERVO
#include "PortServo.hpp"
#endif

#ifndef NO_STEPPER
#include "PortStepper.hpp"
#endif

#include "Accessories.hpp"
