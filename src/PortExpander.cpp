#include "Accessories.h"

#ifndef NO_EXPANDER

PortExpander *PortExpander::pFirstExpander = NULL;

PortExpander::PortExpander()
{
	this->id = (int) UNDEFINED_ID;
	this->pNextExpander = NULL;
}

void PortExpander::Add(PortExpander *inExpander)
{
	if (PortExpander::pFirstExpander == NULL)
	{
		PortExpander::pFirstExpander = inExpander;
		inExpander->SetNextExpander(NULL);
	}

	PortExpander *pCurr = PortExpander::pFirstExpander;

	while (pCurr->GetNextExpander() != NULL)
		pCurr = pCurr->GetNextExpander();

	pCurr->SetNextExpander(inExpander);
	inExpander->SetNextExpander(NULL);
}

PortExpander *PortExpander::GetById(int inId)
{
	PortExpander *pCurr = PortExpander::pFirstExpander;

	while (pCurr != NULL)
	{
		if (pCurr->id == inId)
			return pCurr;
		pCurr = pCurr->GetNextExpander();
	}

	return NULL;
}

#ifdef ACCESSORIES_DEBUG_MODE
void PortExpander::CheckAllExpanderPins()
{
	PortExpander* pCurr = PortExpander::pFirstExpander;

	while (pCurr != NULL)
	{
		for (int i = 0; i < pCurr->GetPinsNumber(); i++)
			if (GPIO_to_Arduino_pin((GPIO_pin_t)EXPANDER_PIN(pCurr->id, i)) != -1)
			{
				Serial.print(F("Warning : the pin "));
				Serial.print(i);
				Serial.print(F(" of the Expander "));
				Serial.print(pCurr->id);
				Serial.println(F(" cannot be used : conflict with internal pin numbering."));
			}

		pCurr = pCurr->GetNextExpander();
	}
}

void PortExpander::CheckExpanderId(int inId)
{
	PortExpander *pExp = PortExpander::GetById(inId);

	if ( pExp == NULL)
	{
			Serial.print(F("Expander "));
			Serial.print(inId);
			Serial.println(F(" unknown !"));
	}
}
#endif

void PortExpander::beginPin(int inPin, int inExpId, PIN_TYPE inType)
{
	if (GPIO_to_Arduino_pin((GPIO_pin_t)EXPANDER_PIN(inExpId, inPin)) != -1)
	{
		Serial.print(F("Warning : the pin "));
		Serial.print(inPin);
		Serial.print(F(" of the Expander "));
		Serial.print(inExpId);
		Serial.println(F(" cannot be used : conflict with internal pin numbering."));
	}

	PortExpander *pCurr = PortExpander::GetById(inExpId);
	if (pCurr != NULL)
		pCurr->beginPin(inPin, inType);
}

void PortExpander::digitalWrite(int inPin, int inExpId, int inValue)
{
	PortExpander *pCcurr = PortExpander::GetById(inExpId);
	if (pCcurr != NULL)
		pCcurr->digitalWrite(inPin, inValue);
}

void PortExpander::analogWrite(int inPin, int inExpId, int inValue)
{
	PortExpander *pCurr = PortExpander::GetById(inExpId);
	if (pCurr != NULL)
		pCurr->analogWrite(inPin, inValue);
}
#endif