/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Sample for four 3 leds signals on a Uno>
*************************************************************/

#include "Commanders.h"
#include "Accessories.h"

#define NB_LEDS		3
#define NB_FEUX		4
#define DCCSTART	20

//------------------------------------------------------------------------------
// SignalArduino declaration

class SignalArduino : public AccessoryLightMulti
{
public:
	inline SignalArduino() {}
	void beginSignal(DriverArduino *inpDriver, uint8_t inNbLeds, int *inpPins, int inStartingDcc);
};

//------------------------------------------------------------------------------
// SignalArduino definition

void SignalArduino::beginSignal(DriverArduino *inpDriver, uint8_t inNbLeds, int *inpPins, int inStartingDcc)
{
	this->begin(0, NB_LEDS, 0);

	for (int led = 0; led < inNbLeds; led++)
	{
		DriverPort *pPort = inpDriver->AddPortMotor(inpPins[led], DIGITAL);
		this->beginLight(led, pPort);
	}

	// Used dcc codes are
	//                 Led  0    1    2
	// inStartingDcc   / 0  on  off  off
	// inStartingDcc   / 1  off on   off
	// inStartingDcc+1 / 0  off off  on
	// inStartingDcc+1 / 1  off off  off

	this->AdjustMovingPositionsSize(inNbLeds + 1);

	int dcc = inStartingDcc;
	bool etat = false;
	for (int i = 0; i < inNbLeds; i++)
	{
		if (!etat)
		{
			this->AddMovingPosition(DCCINT(dcc, 0), 1<<i, 0);
			etat = true;
		}
		else
		{
			this->AddMovingPosition(DCCINT(dcc, 1), 1<<i, 0);
			dcc++;
			etat = false;
		}
	}
	
	this->AddMovingPosition(DCCINT(dcc, etat==true?1:0), 0, 0);

	// Light on at startup
	/*
	for (int i = 0; i < inNbLeds; i++)
	{
		this->LightOn(i);
		unsigned long start = millis();
		while (millis() - start < 1000)
			Accessories::loop();
	}
	*/
	for (int i = 0; i < inNbLeds; i++)
		this->LightOff(i);
}

//------------------------------------------------------------------------------
// Classic INO area

// Commanders

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard	push;
#else
ButtonsCommanderPush push;
#endif

// Accessories

SignalArduino signaux[NB_FEUX];

// Drivers

DriverArduino arduino;

int pins[NB_FEUX][NB_LEDS] = {
	{ 22, 23, 24 },
	{ 25, 26, 27 },
	{ 28, 29, 30 },
	{ 31, 32, 33 },
};

int dcc_codes[NB_FEUX] = { 10, 12, 14, 16 };

void ReceiveEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEventType, int inEventData)
{
	Accessories::ReceiveEvent(inId, (ACCESSORIES_EVENT_TYPE)inEventType, inEventData);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Setup
//
void setup()
{
	Serial.begin(115200);
	while (!Serial);		// For Leonardo only. No effect on other Arduino.

	Commanders::begin(ReceiveEvent, LED_BUILTIN);
	Accessories::begin();

	// Commanders setup

	DccCommander.begin(0x00, 0x00, digitalPinToInterrupt(3));

	// Small push button to check all signal states manually.
	// UNDEFINED_ID here means that this id is not significant.
#ifdef VISUALSTUDIO
	push.begin(UNDEFINED_ID, '0');
#else
	push.begin(UNDEFINED_ID, 17);
#endif

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		int dcc = dcc_codes[feu];
		bool etat = false;
		for (int i = 0; i < NB_LEDS; i++)
		{
			if (!etat)
			{
				push.AddEvent(DCCINT(dcc, 0));
				etat = true;
			}
			else
			{
				push.AddEvent(DCCINT(dcc, 1));
				dcc++;
				etat = false;
			}
		}
		
		push.AddEvent(DCCINT(dcc, etat == true ? 1 : 0));
	}

	arduino.begin ();

	for (int feu = 0; feu < NB_FEUX; feu++)
	{
		signaux[feu].beginSignal(&arduino, NB_LEDS, pins[feu], dcc_codes[feu]);
	}
}

void loop()
{
	Accessories::loop();

	Commanders::loop();
}
