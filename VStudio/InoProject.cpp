#include "Arduino.h"
#include "DIO2.h"

//#include "../examples/CANCommander/CANCommander.ino"
//#include "../examples/Demo/Demo.ino"
#include "../examples/Full/Full.ino"
//#include "../examples/Group/Group.ino"
//#include "../examples/LightFading/LightFading.ino"
//#include "../examples/Servos/Servos.ino"
//#include "../examples/SignalFrench/SignalFrench.ino"
//#include "../examples/Signals4x3/Signals4x3.ino"
//#include "../examples/SimpleLed/SimpleLed.ino"
//#include "../examples/SimpleLedMulti/SimpleLedMulti.ino"

//#include "TestExe.ino"

using namespace System;
using namespace System::Windows::Forms;

void OnMouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);
void OnClick(System::Object ^sender, System::EventArgs ^e);
[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ArduiEmulator::ArduinoForm form(setup, loop);
	form.buttonStart->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(&OnMouseDown);
		
	ArduiEmulator::Arduino::_pinSetup();
	Application::Run(%form);
}

void OnMouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e)
{
	ArduiEmulator::Arduino::_pinSetup();
}
