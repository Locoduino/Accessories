//-------------------------------------------------------------------
#ifndef __accessoryServo_H__
#define __accessoryServo_H__
//-------------------------------------------------------------------

#include "Accessory.hpp"

//-------------------------------------------------------------------

#ifndef NO_SERVO

#define MINIMUM		STATE_FIRST
#define MAXIMUM		STATE_SECOND
#define UNDEFINED	STATE_NONE

// As servo can move with four ways:

enum MovementSpeed
{
	// In fast movement, the full movement is done as fast as possible, in one servo function call. There is no delay, no wait.
	// During the movement, other accessories cannot answer to user action.
	ServoFast,

	// The movement is done degree by degree, with a small delay (less than 5ms) at the end of each movement.
	// During the movement, other accessories cannot answer to user action.
	ServoAlone,

	// The movement is done degree by degree, with a medium delay (6ms to 19ms) at the end of each movement.
	// During the movement, other accessories cannot answer to user action, but these actions are stored in a stack
	// to be executed when the movement will end.
	ServoActionStack,

	// The movement is done degree by degree, with a long delay (greater than or equal to 20ms) at the end of each movement.
	// During the movement, other accessories can handle user actions and move if necessary. This is also the only way to
	// move more than one servo at the same time.
	ServoSlow
};

enum POWER_STATE
{
	PowerNoAction,
	PowerBeforeRunning,
	PowerRunning,
	PowerAfterRunning
};

// This class describes a servo powered by a driver.

class AccessoryServo : public Accessory
{
	protected:
		int currentPosition;
		int minimumPosition;
		int maximumPosition;
		int targetSpeed;
		int targetPosition;
		GPIO_pin_t powerCommandPin;
		unsigned long powerDelay;
		POWER_STATE powerState;

	public:
		AccessoryServo();

		inline int GetMinimumPosition() const { return this->minimumPosition; }
		inline int GetMaximumPosition() const { return this->maximumPosition; }
		inline int GetCurrentPosition() const { return this->currentPosition; }
		inline void SetMinMax(int inMinimum, int inMaximum) { this->minimumPosition = inMinimum; this->maximumPosition = inMaximum; }
		
		inline bool IsMinimumOrLowerPosition() const { return this->currentPosition <= this->minimumPosition; }
		inline bool IsMaximumOrGreaterPosition() const { return this->currentPosition >= this->maximumPosition; }
		inline bool IsMiscPosition() const { return this->currentPosition != this->maximumPosition && this->currentPosition != this->maximumPosition; }
		MovementSpeed GetMovementSpeed() const;

		inline bool CanBePositionnal() const { return true; }

		void begin(DriverPort *inpPort, unsigned long inDurationMilli = 0, int inMinimumPosition = 0, int inMaximumPosition = 180, int inMovingPositionsNumber = 1);
		void SetPowerCommand(int inPin, unsigned long delay = 100);
		void Event(unsigned long inId, ACCESSORIES_EVENT_TYPE inEvent = ACCESSORIES_EVENT_TOGGLE, int inData = 0);

	private:
		void MovePosition(int inPosition);
		void SetState(ACC_STATE instate);
		void Move(unsigned long inId);
		void MoveMinimum();
		void MoveMaximum();
		ACC_STATE MoveToggle();
		inline ACC_STATE Toggle() { return MoveToggle(); }
		bool ActionEnded();
		void InternalMovePosition(int inPosition);
};
#endif

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------

