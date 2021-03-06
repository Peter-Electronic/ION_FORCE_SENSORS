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
#define num_sen0 2
DFRobot_ADS1115 ads(&Wire);

void adc_setup(void) 
{
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
    ads.setGain(eGAIN_FOUR);   
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x49
    ads.setGain(eGAIN_FOUR);   
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();
}
void adc_calib()
{
ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
    if (ads.checkADS1115())
    {
        for(int i=0;i<num_sen0;i++){
          sensor[i].calib = ads.calib(i);
          Serial.print("Calib ADC1 A");
          Serial.print(i);
          Serial.print(":");
          Serial.print(sensor[i].calib);
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
       for(int i=num_sen0;i<num_sensor;i++){
           sensor[i].calib = ads.calib(i-num_sen0);
          Serial.print("ADC2 A");
          Serial.print(i);
          Serial.print(":");
          Serial.print(sensor[i].calib);
          Serial.print("mV\t");
    }
    }
    else
    {
        Serial.println("ADS1115 2 Disconnected!");
    }
    Serial.println();
    delay(1000);
}
void adc_loop() 
{
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
    if (ads.checkADS1115())
    {
        for(int i=0;(i<num_sen0);i++){
//           sensor[i].value = ads.readVoltage(i)-sensor[i].calib;
// sens[i]=sensor[i].value;
//           Serial.print("ADC1 A");
//           Serial.print(i);
//           Serial.print(":");
//           Serial.print(sensor[i].value);
//           Serial.print("mV\t");
if(i==0){
               sensor[i].value=ads.readVoltage(i);
               continue;
           }
sens[i-1] = ads.readVoltage(i)-sensor[0].value;

    }
    }
    else
    {
        Serial.println("ADS1115 1 Disconnected!");
    }
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x49
    if (ads.checkADS1115())
    {
       for(int i=num_sen0-1;i<num_sensor;i++){
           sens[i] = ads.readVoltage(i)-sensor[0].value;
          //  sensor[i].value = ads.readVoltage(i-num_sen0)-sensor[i].calib;
          // Serial.print("ADC2 A");
          // Serial.print(i);
          // Serial.print(":");
          // Serial.print(sensor[i].value);
          // Serial.print("mV\t");
          // sens[i]=sensor[i].value;
    }
    }
    else
    {
        Serial.println("ADS1115 2 Disconnected!");
    }

    Serial.println();
}