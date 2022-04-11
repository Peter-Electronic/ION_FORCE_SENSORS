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
long prevMillisSerial = 0;
int intervalSerial = 500;
    int i;
#include "utilities.h"
#include "boards.h"
//#include "ADC.h"
#include "PumpSwitch.h"
#include "lora.h"

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
    delay(1500);
    Lora_Setup();
    init_ads1115();
    prevMillisSensor = millis();
    motor_setup();
    calib();
}
void loop()
{
    static float sens[num_sensor];
    if (millis() - prevMillisSensor > intervalSensor)
    {
        prevMillisSensor = millis();
        for(int i=0;i<num_sensor;i++){
        adc0.setMultiplexer(uint16_t(i));
        adc0.triggerConversion();
        pollAlertReadyPin();
        // sensor[i].value = adc0.getMilliVolts(false)-sensor[i].calib;
        sensor[i].value = (float)rand()/RAND_MAX;
        sens[i]=sensor[i].value;
        }
    }
    SenderLoop(sens,num_sensor);
    motor_loop();
}
