//-------------------------------------------------------------------
#ifndef __driverPortServoArduino_H__
#define __driverPortServoArduino_H__
//-------------------------------------------------------------------

#ifndef NO_SERVO
#if defined VISUALSTUDIO
class Servo
{
public:
	Servo() { this->pin = -1;  }

	void attach(int inPin) { analogRead(inPin);  this->pin = inPin; }
	void write(int) {}
	int read() { return 0; }
	bool attached() { return this->pin != -1; }

	int servoIndex;
	int pin;
};
#else
#include "Servo.h"
#endif
#endif

#include "Accessories.h"

//-------------------------------------------------------------------

#ifndef NO_SERVO
class DriverPortServoBase : public DriverPortServo
{
	protected:      
		int pin;    
		Servo servo;

	public:
		DriverPortServoBase(uint8_t inId);
		
		void begin(int inPin, PORT_TYPE inType);
		void beginByAccessory(int inStartingPosition);

		inline bool IsActive() const { return this->pin > -1; }
		inline int GetPin() const { return this->pin; }
		inline GPIO_pin_t GetPin2() const { return Arduino_to_GPIO_pin(this->pin); }
		
		void MovePosition(unsigned long inDuration, int inEndPosition);
		int GetPosition();
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
