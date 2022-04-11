#define BLYNK_TEMPLATE_ID "TMPLiKsn-kEk"
#define BLYNK_DEVICE_NAME "ADCsensor"
#define BLYNK_AUTH_TOKEN "1NGbAJmj3xNn2F03x0oNPcQVsB-d0WJe"
// Comment this out to disable prints and save space
#define Blynk_ON
#define BLYNK_PRINT Serial
// int ADC = 25;
#define num_sensor 4
long prevMillisBlynk = 0;
int intervalBlynk = 500;
long prevMillisSerial = 0;
int intervalSerial = 100;
    int ii;
    float sens[num_sensor];
#include "utilities.h"
#include "boards.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "lora.h"
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
    Lora_Setup();
    #ifdef Blynk_ON
    Blynk.begin(auth, ssid, pass);
    #endif
    prevMillisBlynk = millis();
}
void loop()
{
    ReceiverLoop(sens,num_sensor);
    if (millis() - prevMillisSerial > intervalSerial)
    {
        prevMillisSerial = millis();
        for(int i=0;i<num_sensor;i++){
        Serial.print(random(3), 3); // Serial.print("mV\t");
        if(i==3) break;
        Serial.print("\t");
        }
        // Serial.print(random(5)); // Serial.print("mV\t");
        // Serial.print("\t");
        // Serial.print(random(5)); // Serial.print("mV\t");
        // Serial.print("\t");
        // Serial.print(random(5)); // Serial.print("mV\t");
        // Serial.print("\t");
        // Serial.print(random(5)); // Serial.print("mV\t");
        Serial.print(";");

    }
    #ifdef Blynk_ON
    Blynk.run();
    if (millis() - prevMillisBlynk > intervalBlynk)
    {
        // Up data to Blynk
        // Serial.print("\t Publish pressure data to Blynk V0\t");
        prevMillisBlynk = millis();
        if(ii<(num_sensor-1)){
            ii++;
        Blynk.virtualWrite(ii, sens[ii]);
        }
        else{
        ii=0;
        Blynk.virtualWrite(ii, sens[ii]);
        }
    }
    #endif
}
