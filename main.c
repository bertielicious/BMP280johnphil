/*This project implements the BMP280 temperature and pressure sensor, displaying the readings 
 * on a SH1106 oled display, running with dc power supply of 3.3V. Device set up is hand held device, mode = normal, osrs_p = x4, osrs_t = x1, IIRfilter coeff = 16
 * Algorithm
 * Reset the BMP280 by writing 0xB6 to register 0x0E0
 * Read the chipID, and check that a valid ID is received(0x58 or 88d) by reading a byte from register 0xD0
 * If we read a valid chipID, display the chipID on the oled for a few seconds
 * If we did not receive a valid chipID display the following message on the oled "BMP280 not found check wiring and address byte (write 0xEC, read 0xED)" wait here indefinitely
 * write 0x2f to ctrl_meas register 0xF4 which sets up the following conditions: osrs_t = 001 , osrs_p =011 , mode 11 = 0x2f
 * write 0x10 to config register 0xF5 which sets up the following conditions:  t_sb = 000, filer 100, spi3w_en = 0x10
 * create a function checkStatus to check bit 3(1 = conversion in progress) and/ or bit 0(1 = trim parameters copying in progress)
 * checkStatus(bit0)
 * if bit0 == 1, wait for it to become 0
 * if bit0 == 0, burst read all of the trim parameters and store the 16 bit pairs  in twelve 16 bit signed/ unsigned  global variables
 * in main or isr
 * checkStatus(bit3)
 * if(bit3 == 1) wait for it to become 0
 * burst read 20 bit data pressure and temperature registers starting with register address 0xF7 and ending with register0xFC (F7(msb), F8, F9(xlsb), FA(msb), FB, FC(xlsb))
 * use 32 bit floating point compensation formulae to calculate temp and pressure
 * pass temp and pressure value to a function that accepts a 32 bit value, and splits the value into 10 integers
 * pass the 10 pressure and temperature integers to a function that creates an (8 bit) bit map of each number to display to the oled
 * pass the ten integers for temp and pressure to a function that displays the values on the oled display
 * repeat  from line 11
 
 */
/*
 * File:   main.c
 * Author: User
 *
 * Created on 26 April 2022 21:00
 * 
 * 
 * *                  16f1459
 *                  ---------
 *   +5 Volts    1 |Vdd      | 20 0 Volts
    DOWN BUTTON  2 |RA5   RA0| 19   
 *    UP BUTTON  3 |RA4   RA1| 18   
      MCLR       4 |RA3      | 17  
 *               5 |RC5   RC0| 16 
 *               6 |RC4   RC1| 15  
 *               7 |RC3   RC2| 14  
 *               8 |RC6   RB4| 13  SDA
  DIAGNOSTIC LED 9 |RC7   RB5| 12  ENT BUTTON
 *    TX        10 |RB7   RB6| 11  SCL
 *                  ---------
 * 
 * 
 */
#include "config.h"
#include "configOsc.h"
#include "configPorts.h"
#include "configEUSART.h"
#include "configBaud.h"
#include "configI2C.h"
#include "i2cCheckBusIdle.h"
#include "i2cStart.h"
#include "i2cWrite.h"
#include "i2cRead.h"
#include "i2cReStart.h"
#include "i2cStop.h"
#include "i2cWriteSequence.h"
#include "i2cSingleReadSequence.h"
#include "i2cMultipleReadTwo.h"
#include "i2cMultipleReadThree.h"
#include "calcTemp.h"
#include "checkBusy.h"
#include "main.h"

void main(void)
{
    uint8_t result = 0;
    uint8_t varBusy = 1;
    caliTemp calib;
    configOsc();
    configPorts();
    configEUSART();
    configBaud();
    configI2C();
    
    i2cWriteSequence(0xEC, 0xE0, 0xB6);// reset BMP280
    result = i2cSingleReadSequence(0xd0);   // read chipID of BMP280
    if(result == 88)
    {
        printf("chipID %d\n", result);
       
    }  
       
    else if(result !=88)
    {
         printf("BMP280 not detected\n");
         while(1);
    }  
    i2cWriteSequence(0xEC, 0xF4, 0x2f); //write 0x2f to ctrl_meas register 0xF4 which sets up the following conditions: osrs_t = 001 , osrs_p =011 , mode 11 = 0x2f
    i2cWriteSequence(0xEC, 0xF5, 0x10); //write 0x10 to config register 0xF5 which sets up the following conditions:  t_sb = 000, filer 100, spi3w_en = 0x10
    checkBusy(0xF3, varBusy, 0x01);
    calib.digT1 = (uint16_t)i2cMultipleReadTwo(0x88);
    printf("calib.digT1 %x\n", calib.digT1);
    checkBusy(0xF3, varBusy, 0x01);
    calib.digT2 = (int16_t)i2cMultipleReadTwo(0x8A);
    printf("calib.digT2 %x\n", calib.digT2);
    checkBusy(0xF3, varBusy, 0x01);
    calib.digT3 = (int16_t)i2cMultipleReadTwo(0x8C);
    printf("calib.digT3 %x\n", calib.digT3);
    
   /* T1 = (uint16_t)i2cMultipleReadTwo(0x88);
    printf("T1 %x\n", T1);
    
    T2 = (int16_t)i2cMultipleReadTwo(0x8A);
    printf("T2 %x\n", T2);
   
    T3 = (int16_t)i2cMultipleReadTwo(0x8C);
    printf("T3 %x\n", T3);*/
   

    while(1)
    {
        adcT = i2cMultipleReadThree(0xFA);
        printf("adcT %"PRId32"\n", (int32_t)adcT);
        printf("msb %u   lsb  %u   xlsb  %u\n", regMsb, regLsb, regxLsb);
        int32_t temperature = calcTemp(calib, adcT);
        printf("temperature %"PRId32"\n", temperature);
        __delay_ms(1000);
        DIAGNOSTIC_LED = ~DIAGNOSTIC_LED;
    }
}
