//-------------------------------------------------------------------
#ifndef __driverLMD18200_H__
#define __driverLMD18200_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

#ifndef NO_LMD18200

class DriverLMD18200 : public Driver
{
	private:

	public:
		DriverLMD18200();
		
	public:
		inline void begin() {}
		DriverPortLMD18200 *beginPortMotor(int inPinPWM, int inPinDir, int inPinBrake);
};
#endif


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
