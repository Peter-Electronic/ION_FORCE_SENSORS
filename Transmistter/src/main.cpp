
long prevMillisSensor = 0;
int intervalSensor = 100;
long prevMillisSerial = 0;
int intervalSerial = 500;
#include "utilities.h"
#include "boards.h"
#include "ADAFRUIT.h"
#include "PumpSwitch.h"
#include "lora.h"
//#include "DFROBOT.h"

// int ADC = 25;
#define num_sensor 4
struct Sensor_value
{
    float value;
    float calib;
    int sample;
} sensor_default = {0, 0, 0};
// float val0[2], val1[2], val2[2];
struct Sensor_value sensor[num_sensor];
float sens[num_sensor];

void setup()
{
    initBoard();
    delay(1500);
    Lora_Setup();
    adc_setup();
    prevMillisSensor = millis();
    motor_setup();
    // adc_calib();
    //Serial.print("PASS1");
}
void loop()
{
    //Serial.print("PASS2");

    if ((millis() % (PUMP_TIME * 2)) > PUMP_TIME)
    {
        digitalWrite(PUMPPIN, LOW);
        if ((millis() % (PUMP_TIME * 2)) > 3 * PUMP_TIME / 2)
        {
            adc_loop(sens, num_sensor);
            for (byte i = 0; i < num_sensor; i++)
            {
                // sens[i]=(i-2)*100+random(10);
                Serial.print(sens[i]);
                Serial.print(",");
            }
        Serial.println();
            
            SenderLoop(sens, num_sensor);
        }
        else
            delay(1);
    }
    else
    {
        digitalWrite(PUMPPIN, HIGH);
        // SenderLoop(sens, num_sensor);
    }
    // for(int i=0;i<num_sensor;i++){
    //     Serial.print(sens[i]);
    //     Serial.print("\t");
    // }
    // Serial.println();
    // motor_loop();
}
