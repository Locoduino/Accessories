//-------------------------------------------------------------------
#ifndef __driverport_H__
#define __driverport_H__
//-------------------------------------------------------------------

#define DEFAULTSPEED	255
#define DEFAULTDURATION	100

enum PORT_TYPE { DIGITAL, DIGITAL_INVERTED, ANALOG, ANALOG_INVERTED };
enum PORT_STATE { PORT_STOP, PORT_LEFT, PORT_RIGHT };

class DriverPort
{
	protected:
		PORTTYPE type;
		byte id;
		PORT_STATE state;
		int speed;
	
	public:
		DriverPort() {}
		DriverPort(PORTTYPE inType, byte inPort);
		
		inline virtual void begin() {}
		inline virtual void beginByAccessory(int inStartingPosition) {}

		inline PORT_STATE GetState() const { return this->state; }
		inline int GetSpeed() const { return this->speed; }
		inline PORTTYPE GetType() const { return this->type; }
		inline byte GetId() const { return this->id; }
		virtual int SetSpeed(int inSpeed);
		
		inline bool IsLeftDir() const { return this->state == PORT_LEFT; }
		inline bool IsRightDir() const { return this->state == PORT_RIGHT; }
		inline bool IsStopped() const { return this->state == PORT_STOP; }
		
		inline virtual void MoveLeftDir(unsigned long inDuration = 0) { this->state = PORT_LEFT; }
		inline virtual void MoveRightDir(unsigned long inDuration = 0) { this->state = PORT_RIGHT; }
		void MoveLeftDir(unsigned long inDuration, int inSpeed);
		void MoveRightDir(unsigned long inDuration, int inSpeed);
		PORT_STATE MoveToggle(unsigned long inDuration = 0);
		inline virtual void MoveStop()	{ this->state = PORT_STOP; }
		inline virtual void MovePosition(unsigned long inDuration, int inEndPosition) {}
		inline virtual int GetPosition() { return 0; }
};


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
