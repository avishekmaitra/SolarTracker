#ifndef I2C_H_
#define I2C_H_
#define ACCEL_ADDRESS 0x1D
#include <stdint.h>

void InitI2C(uint8_t DeviceAddress);
void WriteI2C_SingleByte(uint8_t MemAddress, uint8_t MemByte);
uint8_t ReadI2C_SingleByte(uint8_t MemAddress);
void WriteI2C_MultiByte(uint8_t MemAddress, uint16_t MemByte);
uint16_t ReadI2C_MultiByte(uint8_t MemAddress);

#endif /* I2C_H_ */
