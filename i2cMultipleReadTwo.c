#include "config.h"


#include "i2cSingleReadSequence.h"
#include "checkBusy.h"
uint16_t i2cMultipleReadTwo(uint8_t regAddr)
{
    uint8_t varBusi = 1;
    uint8_t msb, lsb;
    checkBusy(0xF3, varBusi, 0x01);
    lsb = i2cSingleReadSequence(regAddr);
   // __delay_ms(100);
    checkBusy(0xF3, varBusi, 0x01);
    msb = i2cSingleReadSequence(regAddr + 1);
    __delay_ms(100);
  
   // temp = ((uint16_t)msb << 8) | (uint16_t)lsb;
    return ((uint16_t)msb << 8) | lsb;
    //return (msb << 8) | lsb;
}