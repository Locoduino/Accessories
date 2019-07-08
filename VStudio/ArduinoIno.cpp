// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>

#include "pch.h"
#include "Arduino.h"
#include "DIO2.h"
#include "ArduiEmulator.hpp" 
#include "NamedPipes.h" 

//#include "../examples/CANCommander/CANCommander.ino"
//#include "../examples/Demo/Demo.ino"
//#include "../examples/Full/Full.ino"
//#include "../examples/Group/Group.ino"
//#include "../examples/LightFading/LightFading.ino"
//#include "../examples/Servos/Servos.ino"
//#include "../examples/SignalFrench/SignalFrench.ino"
//#include "../examples/Signals4x3/Signals4x3.ino"
//#include "../examples/SimpleLed/SimpleLed.ino"
//#include "../examples/SimpleLedMulti/SimpleLedMulti.ino"
//#include "../examples/Stepper/Stepper.ino"
//#include "../examples/LMD18200/LMD18200.ino"
#include "../examples/74HC595/74HC595.ino"
//#include "../examples/SX1509/SX1509.ino"
//#include "../examples/locoduino.org/programme1/programme1.ino"
//#include "../examples/locoduino.org/programme2/programme2.ino"
//#include "../examples/locoduino.org/programme3/programme3.ino"
//#include "../examples/locoduino.org/programme4/programme4.ino"
//#include "../examples/locoduino.org/programme5/programme5.ino"
////#include "../examples/locoduino.org/programme6/programme6.ino"
//#include "../../../TPC/Leds/Leds.ino"

//#include "TestExe.ino"

int main()
{
	NamedPipesBegin(L"\\\\.\\pipe\\ToArduino", L"\\\\.\\pipe\\FromArduino");
	
	NamedPipesSend(MessagesTypes::Reset, _T(""));

	pinSetup();

	setup();

	while (1)
	{
		NamedPipesIdle();
		loop();
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
