/************************************************************** 
  This is a library for the PCA9505/06 40bit i2c port expander
 *************************************************************/

#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include "PCA9505_9506.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

void PCA9505_06::begin(uint8_t addr) {
	i2caddr = addr;

	Wire.begin(addr);
	
#if 0
	setIOInterrupt((IOPORTS){
		0xFF /* Port 0 */,
		0xFF /* Port 1 */,
		0xFF /* Port 2 */,
		0xFF /* Port 3 */,
		0xFF /* Port 4 */
	});
	
	setIOPolarity((IOPORTS){
		0x00 /* Port 0 */,
		0x00 /* Port 1 */,
		0x00 /* Port 2 */,
		0x00 /* Port 3 */,
		0x00 /* Port 4 */
	});
	ioMode((IOPORTS){
		0x00 /* Port 0 */,
		0x00 /* Port 1 */,
		0x00 /* Port 2 */,
		0x00 /* Port 3 */,
		0x00 /* Port 4 */
	});
#endif
}

void PCA9505_06::begin(void) {
	begin(PCA9505_BASE_ADDRESS);
}

uint8_t PCA9505_06::getPort(uint8_t pin) {
	return pin / 8;
}

uint8_t PCA9505_06::getBit(uint8_t pin) {
	return pin % 8;
}

void PCA9505_06::sendPortData(uint8_t reg, uint8_t port, uint8_t data) {
	Wire.beginTransmission(i2caddr);
	Wire.write((reg + (port % 5)) | PCA9505_AI_OFF);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t PCA9505_06::retrievePortData(uint8_t reg, uint8_t port) {
	Wire.beginTransmission(i2caddr);
	Wire.write((reg + (port % 5)) | PCA9505_AI_OFF);
	Wire.endTransmission();
	
	Wire.requestFrom(i2caddr, 1);
	
	return Wire.read();
}

void PCA9505_06::sendIOData(uint8_t reg, IOPORTS io) {
	Wire.beginTransmission(i2caddr);
	Wire.write(reg | PCA9505_AI_ON);
	Wire.write(io.port[0]);
	Wire.write(io.port[1]);
	Wire.write(io.port[2]);
	Wire.write(io.port[3]);
	Wire.write(io.port[4]);
	Wire.endTransmission();
}

IOPORTS PCA9505_06::retrieveIOData(uint8_t reg) {
	IOPORTS data = { 0 };
	Wire.beginTransmission(i2caddr);
	Wire.write(reg | PCA9505_AI_ON);
	Wire.endTransmission();
	
	Wire.requestFrom(i2caddr, 5);
	
	data.port[0] = Wire.read();
	data.port[1] = Wire.read();
	data.port[2] = Wire.read();
	data.port[3] = Wire.read();
	data.port[4] = Wire.read();
	return data;
}

void PCA9505_06::pinMode(uint8_t pin, uint8_t dir) {
	uint8_t set = retrievePortData(PCA9505_IOC, getPort(pin));
	bitWrite(set, getBit(pin), (dir ? 0 : 1));
	sendPortData(PCA9505_IOC, getPort(pin), set);
}

uint8_t PCA9505_06::getPinMode(uint8_t pin) {
	return (retrievePortData(PCA9505_IOC, getPort(pin)) & (1 << getBit(pin))) ? INPUT : OUTPUT;
}

void PCA9505_06::portMode(uint8_t port, uint8_t dir) {
	sendPortData(PCA9505_IOC, port, (dir ? 0x00 : 0xFF));
}

uint8_t PCA9505_06::getPortMode(uint8_t port) {
	return ~retrievePortData(PCA9505_IOC, port);
}

void PCA9505_06::ioMode(IOPORTS io) {
	io.port[0] = ~io.port[0];
	io.port[1] = ~io.port[1];
	io.port[2] = ~io.port[2];
	io.port[3] = ~io.port[3];
	io.port[4] = ~io.port[4];
	sendIOData(PCA9505_IOC, io);
}

IOPORTS PCA9505_06::getIoMode() {
	IOPORTS ports = retrieveIOData(PCA9505_IOC);
	ports.port[0] = ~ports.port[0];
	ports.port[1] = ~ports.port[1];
	ports.port[2] = ~ports.port[2];
	ports.port[3] = ~ports.port[3];
	ports.port[4] = ~ports.port[4];
	return ports;
}

void PCA9505_06::digitalWrite(uint8_t pin, uint8_t dir) {
	uint8_t set = retrievePortData(PCA9505_OP, getPort(pin));
	bitWrite(set, getBit(pin), (dir ? HIGH : LOW));
	sendPortData(PCA9505_OP, getPort(pin), set);
}

void PCA9505_06::digitalPortWrite(uint8_t port, uint8_t dir[8]) {
	uint8_t set = 0;
	
	bitWrite(set, 0, (dir[0] ? HIGH : LOW));
	bitWrite(set, 1, (dir[1] ? HIGH : LOW));
	bitWrite(set, 2, (dir[2] ? HIGH : LOW));
	bitWrite(set, 3, (dir[3] ? HIGH : LOW));
	bitWrite(set, 4, (dir[4] ? HIGH : LOW));
	bitWrite(set, 5, (dir[5] ? HIGH : LOW));
	bitWrite(set, 6, (dir[6] ? HIGH : LOW));
	bitWrite(set, 7, (dir[7] ? HIGH : LOW));
	sendPortData(PCA9505_OP, port, set);
}

void PCA9505_06::digitalIOWrite(IOPORTS io) {
	sendIOData(PCA9505_OP, io);
}

uint8_t PCA9505_06::digitalRead(uint8_t pin) {	
	return (retrievePortData(PCA9505_IP, getPort(pin)) & (1 << getBit(pin))) ? HIGH : LOW;
}

uint8_t PCA9505_06::digitalPortRead(uint8_t port) {
	return retrievePortData(PCA9505_IP, port);
}
  
IOPORTS PCA9505_06::digitalIORead() {
	return retrieveIOData(PCA9505_IP);
}

void PCA9505_06::setPinInterrupt(uint8_t pin, bool enable = true) {
	uint8_t set = retrievePortData(PCA9505_MSK, getPort(pin));
	bitWrite(set, getBit(pin), (enable ? 0 : 1));
	sendPortData(PCA9505_MSK, getPort(pin), set);
}

uint8_t PCA9505_06::getPinInterrupt(uint8_t pin) {
	return (retrievePortData(PCA9505_MSK, getPort(pin)) >> getBit(pin)) & 0x01;
}

void PCA9505_06::setPortInterrupt(uint8_t port, uint8_t enable) {
	sendPortData(PCA9505_MSK, port, enable);
}

uint8_t PCA9505_06::getPortInterrupt(uint8_t port) {
	return retrievePortData(PCA9505_MSK, port);
}

void PCA9505_06::setIOInterrupt(IOPORTS io) {
	sendIOData(PCA9505_MSK, io);
}

IOPORTS PCA9505_06::getIOInterrupt() {
	return retrieveIOData(PCA9505_MSK);
}

void PCA9505_06::clearInterrupt() {
	digitalIORead();
}

void PCA9505_06::setPinPolarity(uint8_t pin, uint8_t toggle) {
	uint8_t set = retrievePortData(PCA9505_PI, getPort(pin));
	bitWrite(set, getBit(pin), (toggle ? 1 : 0));
	sendPortData(PCA9505_PI, getPort(pin), set);
}

uint8_t PCA9505_06::getPinPolarity(uint8_t pin) {
	return (retrievePortData(PCA9505_PI, getPort(pin)) >> getBit(pin)) & 0x01;
}

void PCA9505_06::setPortPolarity(uint8_t port, uint8_t toggle) {
	sendPortData(PCA9505_PI, port, toggle);
}

uint8_t PCA9505_06::getPortPolarity(uint8_t port) {
	return retrievePortData(PCA9505_PI, port);
}

void PCA9505_06::setIOPolarity(IOPORTS io) {
	sendIOData(PCA9505_PI, io);
}

IOPORTS PCA9505_06::getIOPolarity() {
	return retrieveIOData(PCA9505_PI);
}