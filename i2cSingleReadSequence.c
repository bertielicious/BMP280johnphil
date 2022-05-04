#include "config.h"
#include "i2cCheckBusIdle.h"
#include "i2cStart.h"
#include "i2cWrite.h"
#include "i2cReStart.h"
#include "i2cStop.h"
#include "i2cRead.h"
uint8_t i2cSingleReadSequence(uint8_t registerAddr)
{
    uint8_t registerContents;
    i2cCheckBusIdle(); // i2c read sequence
    i2cStart();        // issue the start condition
    i2cWrite(0xEC);    //write the BMP280 slave write address of 0xEC(R/W = '0')
    i2cWrite(registerAddr);    // CHIP_ID_REG address 0xD0
    i2cReStart();      // issue a repeated start condition
    i2cWrite(0xEC + 1);     // read slave address BMP280 is 0xED (R/W = '1')
    registerContents = i2cRead();//read the data stored inside register D0
    i2cStop();  // end the communication by issuing a stop condition
    return registerContents;
}
