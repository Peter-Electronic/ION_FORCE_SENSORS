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
#define BLYNK_PRINT Serial
// int ADC = 25;
float val0, val1;
long prevMillisSensor = 0;
int intervalSensor = 100;
long prevMillisBlynk = 0;
int intervalBlynk = 100;
#include <RadioLib.h>
#include "utilities.h"
#include "boards.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "ADC.h"

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi-5470";
char pass[] = "22141460";
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
// This function is called every time the Virtual Pin 0 state changes

void setup()
{
    initBoard();

    // When the power is turned on, a delay is required.
    delay(1500);
    // initialize SX1276 with default settings
    init_ads1115();
    Blynk.begin(auth, ssid, pass);
    // analogSetPinAttenuation(ADC,ADC_0db);
    prevMillisSensor = millis();
    prevMillisBlynk = millis();
}
void loop()
{
    Blynk.run();
    if (millis() - prevMillisSensor > intervalSensor)
    {
        prevMillisSensor = millis();
        adc0.setMultiplexer(ADS1115_MUX_P1_NG);
        adc0.triggerConversion();
        pollAlertReadyPin();
        val0 = adc0.getMilliVolts(false);
        Serial.print("A1: ");
        Serial.print(val0, 3); // Serial.print("mV\t");
        adc0.setMultiplexer(ADS1115_MUX_P2_NG);
        adc0.triggerConversion();
        pollAlertReadyPin();
        val1 = adc0.getMilliVolts(false);
        Serial.print("\tA2: ");
        Serial.print(val1, 3); // Serial.print("mV\t");
        adc0.setMultiplexer(ADS1115_MUX_P3_NG);
        adc0.triggerConversion();
        pollAlertReadyPin();
        val1 = adc0.getMilliVolts(false);
        Serial.print("\tA3: ");
        Serial.println(val1, 3); // Serial.print("mV\t");
    }
    if (millis() - prevMillisBlynk > intervalBlynk)
    {
        // Up data to Blynk
        // Serial.print("\t Publish pressure data to Blynk V0\t");
        Blynk.virtualWrite(0, val0);
        Blynk.virtualWrite(1, val1);
        prevMillisBlynk = millis();
    }
    // You can inject your own code or combine it with other sketches.
    // Check other examples on how to communicate with Blynk. Remember
    // to avoid delay() function!
}
