
long prevMillisSensor = 0;
int intervalSensor = 100;
long prevMillisSerial = 0;
int intervalSerial = 500;
#include "utilities.h"
#include "boards.h"
//#include "ADC.h"
#include "PumpSwitch.h"
#include "lora.h"
#include "DFROBOT.h"

void setup()
{
    initBoard();
    delay(1500);
    Lora_Setup();
    adc_setup();
    prevMillisSensor = millis();
    //motor_setup();
    adc_calib();
}
void loop()
{
    if (millis() - prevMillisSensor > intervalSensor)
    {
        prevMillisSensor = millis();
        // sensor[i].value = (float)rand()/RAND_MAX;
        adc_loop();

    }

    SenderLoop(sens,num_sensor);
    //motor_loop();
}
