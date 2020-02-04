#ifndef I2C_H_
#define I2C_H_
#include <stdint.h>

void InitI2C(uint8_t DeviceAddress);
void WriteI2C(uint8_t MemAddress, uint8_t MemByte);
uint8_t ReadI2C(uint8_t MemAddress);

#endif /* I2C_H_ */
