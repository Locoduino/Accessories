////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
#define ACCESSORIES_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if ACCESSORIES_DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

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
//NO_SERVO
//	AccessoryServo.cpp
//	AccessoryServo.hpp
//	DriverPortServo.cpp
//	DriverPortServo.hpp
//	DriverPortServoBase.cpp
//	DriverPortServoBase.hpp
//	Servo.cpp
//	Servo.hpp
//
//NO_L293D
//	DriverL293d.cpp
//	DriverL293d.hpp
//	DriverPortL293d.cpp
//	DriverPortL293d.hpp
//
//NO_L9110
//	DriverL9110.cpp
//	DriverL9110.hpp
//
//NO_L298D
//	DriverL298n.cpp
//	DriverL298n.hpp
//
//NO_LMD18200
//	DriverLMD18200.cpp
//	DriverLMD18200.hpp
//	DriverPortLMD18200.cpp
//	DriverPortLMD18200.hpp
//
//NO_ULN2003
//	DriverULN2003.cpp
//	DriverULN2003.hpp
//
//NO_RELAY
//	DriverRelay.cpp
//	DriverRelay.hpp
//	DriverPortMotor.cpp
//	DriverPortMotor.hpp
//
//NO_STEPPER
//	AccessoryStepper.cpp
//	AccessoryStepper.hpp
//	DriverStepper.cpp
//	DriverStepper.hpp
//	DriverPortStepper.cpp
//	DriverPortStepper.hpp
//

//#define NO_GROUP
//#define NO_MOTOR
//#define NO_SERVO
//#define NO_STEPPER
//#define NO_LIGHT
//#define NO_L293D
//#define NO_L9110
//#define NO_L298N
//#define NO_ULN2003
//#define NO_LMD18200
//#define NO_RELAY
//#define NO_ARDUINODRIVER

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
#ifndef NO_L293D
	#define NO_L293D
#endif
#endif

/////////////////////////////////////

#include "AccessoriesClass.hpp"
#include "Driver.hpp"
#include "DriverPort.hpp"

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

#include "DriverPortArduino.hpp"
#include "DriverPortMotor.hpp"
#include "DriverPort2Pins.hpp"

#ifndef NO_L293D
#include "DriverPortL293d.hpp"
#endif
#ifndef NO_LMD18200
#include "DriverPortLMD18200.hpp"
#endif
#ifndef NO_SERVO
#include "DriverPortServo.hpp"
#include "DriverPortServoBase.hpp"
#endif
#ifndef NO_STEPPER
#include "DriverPortStepper.hpp"
#endif

#ifndef NO_ARDUINODRIVER
#include "DriverArduino.hpp"
#endif
#ifndef NO_L293D
#include "DriverL293d.hpp"
#endif
#ifndef NO_L9110
#include "DriverL9110.hpp"
#endif
#ifndef NO_L298N
#include "DriverL298n.hpp"
#endif
#ifndef NO_ULN2003
#include "DriverULN2003.hpp"
#endif
#ifndef NO_LMD18200
#include "DriverLMD18200.hpp"
#endif
#ifndef NO_RELAY
#include "DriverRelay.hpp"
#endif

#include "Accessories.hpp"
