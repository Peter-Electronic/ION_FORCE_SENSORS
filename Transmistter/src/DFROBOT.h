/*!
 * @file readVoltageCascade.ino
 * @brief Cascade two ADS1115s to your board and choose different I2C address(please reference board compatibility)
 * @n The voltage value read by A0 A1 A2 A3 is printed through the serial port.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [luoyufeng](yufeng.luo@dfrobot.com),
 * @version  V1.0
 * @date  2019-06-19
 * @url https://github.com/DFRobot/DFRobot_ADS1115
 */

#include <Wire.h>
#include <DFRobot_ADS1115.h>

DFRobot_ADS1115 ads(&Wire);

void adc_setup(void) 
{
    Serial.begin(115200);

    ads.setGain(eGAIN_FOUR);   
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();
}

void adc_loop(float sens[], uint8_t num_sensor) 
{
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
    if (ads.checkADS1115())
    {
        for(int i=0;(i<num_sensor)&&(i<4);i++){
          sens[i] = ads.readVoltage(i);
          Serial.print("ADC1 A");
          Serial.print(i);
          Serial.print(":");
          Serial.print(sen[i]);
          Serial.print("mV\t");
    }
    }
    else
    {
        Serial.println("ADS1115 1 Disconnected!");
    }
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x49
    if (ads.checkADS1115())
    {
       for(int i=4;i<num_sensor;i++){
          sens[i] = ads.readVoltage(i);
          Serial.print("ADC2 A");
          Serial.print(i);
          Serial.print(":");
          Serial.print(sen[i]);
          Serial.print("mV\t");
    }
    else
    {
        Serial.println("ADS1115 2 Disconnected!");
    }
}