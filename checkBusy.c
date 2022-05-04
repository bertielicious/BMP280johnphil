#include "config.h"
#include "i2cSingleReadSequence.h"
void checkBusy(uint8_t regAddress, uint8_t bitStatus, uint8_t testBit)
{
    uint8_t varbusy;
    varbusy = bitStatus;
    
    while((varbusy = i2cSingleReadSequence((regAddress) & testBit) == 0x01))
    {
        ;//wait
    }
    bitStatus = 0;
}
