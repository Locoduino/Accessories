/*************************************************************
project: <Accessories>
author: <Thierry PARIS>
description: <Class for a one way motorized accessory>
*************************************************************/

#include "Accessories.h"

#ifdef ACCESSORIES_PRINT_ACCESSORIES
void AccessoryMotorOneWay::printAccessory()
{
	Serial.print(F("    MotorOneWay : ID "));
	Serial.print(this->GetMovingPositionIdByIndex(0));

	if (this->GetPort() != NULL)
	{
		Serial.print(F(" / Speed: "));
		Serial.print(this->GetPort()->GetSpeed());
	}
	Serial.print(F(" / Delay: "));
	Serial.print(this->GetDebounceDelay());
	Serial.print(F(" / "));
	if (this->GetPort() != NULL)
		this->GetPort()->printPort();
	Serial.println(F(" "));
}
#endif
