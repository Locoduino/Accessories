//-------------------------------------------------------------------
#ifndef __PortPWMDirBrake_H__
#define __PortPWMDirBrake_H__
//-------------------------------------------------------------------

#include "Port.hpp"

/**This class describes a port for a motor driver like a LMD18200.
Three pins will be used, for PWM, dir and brake connectors of the driver.
PWM pin gives the power, the speed,
dir pin gives the direction. One direction is when the pin is LOW, the other when the pin is HIGH.
brake pin stops the motor if it is HIGH.
*/
class PortSpeedDirBrake : public Port
{
	protected:
		/**PWM pin.*/
		int pinPWM;
		/**Type of digital pins dir and brake.*/
		PIN_TYPE digitalType;
		/**Direction pin of this port.*/
		GPIO_pin_t pinDir;
		/**Brake pin of this port.*/
		GPIO_pin_t pinBrake;

		/**Map the digital value (HIGH o LOW) according to the inverted or not type of the pin.
		@param inValue value to map.
		@return returns the adapted value according to the pin type :

		type             | LOW  | HIGH
		-----------------|------|--------
		DIGITAL	         | LOW  | HIGH
		DIGITAL_INVERTED | HIGH | LOW
		*/

		int MapDigitalValue(int inValue) const;

	public:
		/**Default constructor.*/
		PortSpeedDirBrake();
		
		/** Initialize the instance.
		@param inPinPWM Analog pin for power.
		@param inPinDir Digital direction pin.
		@param inPinBrake Brake pin.
		@param inPWMType kind of analog PWM pin. Can be only ANALOG or ANALOG_INVERTED.
		@param inDigitalType kind of both digital pins dir and brake. Can only be DIGITAL or DIGITAL_INVERTED..
		*/
		void begin(int inPinPWM, int inPinDir, int inPinBrake, PIN_TYPE inPWMType, PIN_TYPE inDigitalType);
		
		/**Checks if this port is active. To be active, begin() must have been called.
		@return true if the power pin number (pinPWM) is different than -1.
		*/
		inline bool IsActive() const { return this->pinPWM != -1; }
		/**Gets the PWM pin.
		@return pin number in Arduino mode.
		*/
		inline int GetPinPWM() const { return this->pinPWM; }
		/**Gets the direction pin.
		@return pin number in Arduino mode.
		*/
		inline int GetPinDir() const { return GPIO_to_Arduino_pin(this->pinDir); }
		/**Gets the direction pin.
		@return Pin A number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPinDir() const { return this->pinDir; }
		/**Gets the brake pin.
		@return pin number in Arduino mode.
		*/
		inline int GetPinBrake() const { return GPIO_to_Arduino_pin(this->pinBrake); }
		/**Gets the brake pin.
		@return Pin A number in DIO2 mode.
		*/
		inline GPIO_pin_t GetDIOPinBrake() const { return this->pinBrake; }

		/**Sets the current state of the port to PORT_LEFT.
		@param inDuration the pins will be activated to left for the given delay.
		*/
		void MoveLeftDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_RIGHT.
		@param inDuration the pins will be activated to right for the given delay.
		*/
		void MoveRightDir(unsigned long inDuration = 0);
		/**Sets the current state of the port to PORT_STOP.	*/
		void MoveStop();

#ifdef ACCESSORIES_PRINT_ACCESSORIES
		/** Print this port on console.
		@remark Only available if ACCESSORIES_PRINT_ACCESSORIES is defined.
		*/
		void printPort();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
