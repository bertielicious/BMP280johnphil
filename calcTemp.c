#include "config.h"
int32_t calcTemp(caliTemp trim, uint32_t adct)
{
  int32_t var1 = (((((int32_t)adct >> 3) - ((int32_t)trim.digT1 << 1))) * ((int32_t)trim.digT2)) >> 11;

  int32_t var2 = ((((((int32_t)adct >> 4) - ((int32_t)trim.digT1)) * (((int32_t)adct >> 4) - ((int32_t)trim.digT1))) >> 12) *((int32_t)trim.digT3)) >>14;

  int32_t t_fine = var1 + var2;

  int32_t t = (t_fine * 5 + 128) >> 8;
  
  return t;
}