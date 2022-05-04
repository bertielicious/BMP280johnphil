#include "config.h"
#include "i2cSingleReadSequence.h"
#include "checkBusy.h"
uint32_t i2cMultipleReadThree(uint8_t regAddr)
{
    uint8_t varBusi = 1;
    uint8_t msb, lsb, xlsb;
    uint32_t temp;
    //checkBusy(0xF3, varBusi, 0x08);
    msb = i2cSingleReadSequence(regAddr);
    __delay_ms(100);
   // checkBusy(0xF3, varBusi, 0x08);
    lsb = i2cSingleReadSequence(regAddr + 1);
     __delay_ms(100);
   // checkBusy(0xF3, varBusi, 0x08);
    xlsb = i2cSingleReadSequence(regAddr + 2);
     __delay_ms(100);
   // checkBusy(0xF3, varBusi, 0x08);
    regMsb = msb;
    regLsb = lsb;
    regxLsb = xlsb;
    temp = (((uint32_t)msb<<12) | ((uint32_t)lsb<<4) | ((uint32_t)xlsb>>4));
    return temp;
}
