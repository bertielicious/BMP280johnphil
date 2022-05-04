#include "config.h"
#include "i2cCheckBusIdle.h"
#include "i2cStart.h"
#include "i2cWrite.h"
#include "i2cStop.h"
void i2cWriteSequence(uint8_t writeSlaveAddr, uint8_t registerAddr, uint8_t dataByte)
{
    i2cCheckBusIdle();  // i2c write sequence
    i2cStart();
    i2cWrite(writeSlaveAddr); // write the BMP280 slave write address of 0xEC(R/W = '0')
    i2cWrite(registerAddr); // write the BMP280 reset register address of 0xE0
    i2cWrite(dataByte); // write the BMP280 data byte for a reset of 0xB6
    i2cStop();      // end the communication by issuing a stop condition
}
