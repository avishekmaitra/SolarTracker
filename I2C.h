#ifndef I2C_H_
#define I2C_H_
#define ACCEL_ADDRESS 0x1D
#include <stdint.h>

void I2C_Init(uint8_t DeviceAddress);
void I2C_WriteSingleByte(uint8_t MemAddress, uint8_t MemByte);
uint8_t I2C_ReadSingleByte(uint8_t MemAddress);
void I2C_WriteMultiByte(uint8_t MemAddress, uint16_t MemByte);
uint16_t I2C_ReadMultiByte(uint8_t MemAddress);
uint8_t I2C_GetComErrorFlag();
void I2C_ResetComErrorFlag(void);

#endif /* I2C_H_ */
