/*
   RadioLib SX127x Receive Example

   This example listens for LoRa transmissions using SX127x Lora modules.
   To successfully receive data, the following settings have to be the same
   on both transmitter and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
    - preamble length

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/
#define BLYNK_TEMPLATE_ID "TMPLiKsn-kEk"
#define BLYNK_DEVICE_NAME "ADCsensor"
#define BLYNK_AUTH_TOKEN "1NGbAJmj3xNn2F03x0oNPcQVsB-d0WJe"

// Comment this out to disable prints and save space
#define Blynk_ON
#define BLYNK_PRINT Serial
// int ADC = 25;
#define num_sensor 4
struct Sensor_value{
    float value;
    float calib;
    int sample;
} sensor_default={0,0,0};
// float val0[2], val1[2], val2[2];
struct Sensor_value sensor[num_sensor];
long prevMillisSensor = 0;
int intervalSensor = 100;
long prevMillisBlynk = 0;
int intervalBlynk = 500;
long prevMillisSerial = 0;
int intervalSerial = 500;
    int i;
#include <RadioLib.h>
#include "utilities.h"
#include "boards.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "ADC.h"
#include "PumpSwitch.h"

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi-5470";
char pass[] = "22141460";
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
// This function is called every time the Virtual Pin 0 state changes
void calib(){
    Serial.println("sensor calib");
    for (int i=0;i<num_sensor;i++){
        sensor[i]=sensor_default;
    }
    while (millis() - prevMillisSensor < 5000)
    {
        for(int i=0;i<num_sensor;i++){
            if (i==0)
        adc0.setMultiplexer(ADS1115_MUX_P1_NG);
        else if(i==1)
        adc0.setMultiplexer(ADS1115_MUX_P2_NG);
        else if(i==2)
        adc0.setMultiplexer(ADS1115_MUX_P3_NG);
        adc0.triggerConversion();
        pollAlertReadyPin();
        sensor[i].calib += adc0.getMilliVolts(false);
        sensor[i].sample+=1;
        }
    }
    for(int i=0;i<num_sensor;i++){
    sensor[i].calib=sensor[i].calib/sensor[i].sample;
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" calib: ");
    Serial.println(sensor[i].calib);
    }
}
void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);
    // initialize SX1276 with default settings
    init_ads1115();
    #ifdef Blynk_ON
    Blynk.begin(auth, ssid, pass);
    #endif
    // analogSetPinAttenuation(ADC,ADC_0db);
    prevMillisSensor = millis();
    prevMillisBlynk = millis();
    //calib();
    motor_setup();
    calib();
}
void loop()
{
    if (millis() - prevMillisSensor > intervalSensor)
    {
        prevMillisSensor = millis();
        for(int i=0;i<num_sensor;i++){
        adc0.setMultiplexer(uint16_t(i));
        // Serial.print("A");
        // Serial.print(i);
        // Serial.print(": ");
        adc0.triggerConversion();
        pollAlertReadyPin();
        sensor[i].value = adc0.getMilliVolts(false)-sensor[i].calib;
        }
    }
    if (millis() - prevMillisSerial > intervalSerial)
    {
        prevMillisSerial = millis();
        // for(int i=0;i<num_sensor;i++){
        // Serial.print("\t");
        // Serial.print(sensor[i].value, 3); // Serial.print("mV\t");
        // }
        Serial.print(random(5)); // Serial.print("mV\t");
        Serial.print("\t");
        Serial.print(random(5)); // Serial.print("mV\t");
        Serial.print("\t");
        Serial.print(random(5)); // Serial.print("mV\t");
        Serial.print("\t");
        Serial.print(random(5)); // Serial.print("mV\t");
        Serial.print(";");

    }
    #ifdef Blynk_ON
    Blynk.run();
    if (millis() - prevMillisBlynk > intervalBlynk)
    {
        // Up data to Blynk
        // Serial.print("\t Publish pressure data to Blynk V0\t");
        prevMillisBlynk = millis();
        if(i<(num_sensor-1)){
            i++;
        Blynk.virtualWrite(i, sensor[i].value);
        }
        else{
        i=0;
        Blynk.virtualWrite(i, sensor[i].value);
        }
    }
    #endif
    // You can inject your own code or combine it with other sketches.
    // Check other examples on how to communicate with Blynk. Remember
    // to avoid delay() function!
    motor_loop();
}
