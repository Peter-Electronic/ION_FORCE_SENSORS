#include <RadioLib.h>
#include "utilities.h"
#include <WiFi.h>
#include <WiFiClient.h>
uint32_t loraLoopMillis = 0;
uint32_t loraInterval=1;
SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DI0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
// flag to indicate that a packet was received
volatile bool receivedFlag = false;
// disable interrupt when it's not needed
volatile bool enableInterrupt = true;
// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void)
{
    // check if the interrupt is enabled
    if (!enableInterrupt)
    {
        return;
    }

    // we got a packet, set the flag
    receivedFlag = true;
}
void Lora_Setup()
{
    Serial.print(F("[SX1276] Initializing ... "));
#ifndef LoRa_frequency
    int state = radio.begin(868.0);
#else
    // int state = radio.begin(LoRa_frequency, 10.4, 12, 8, SX127X_SYNC_WORD, 17, 24, 1);
    // int state = radio.begin(137.0);
    int state = radio.begin(LoRa_frequency);

#endif
    if (state == ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }
#ifdef RADIO_USING_SX1262
    radio.setDio1Action(setFlag);
#else
    radio.setDio0Action(setFlag);
#endif

    // start listening for LoRa packets
#ifdef RECEIVER
    radio.startReceive();
#endif
}
void float2byte(float a, byte dataArray[], uint8_t start)
{
    for (int i = 0; i < 4; i++)
    {
        dataArray[start + i] = ((uint8_t *)&a)[i];
    };
}
// and make opposite operation on the receiver side:
void byte2float(float *a, byte dataArray[], uint8_t start)
{
    for (int i = 0; i < 4; i++)
    {
        ((uint8_t *)a)[i] = dataArray[start + i];
    };
}
void ReceiverLoop(float Data[], byte sizeOfData)
{
    // check if the flag is set
    if (receivedFlag)
    {
        enableInterrupt = false;
        receivedFlag = false;
        byte byteArr[sizeOfData * 4];
        int state = radio.readData(byteArr, sizeOfData * 4);
        if (state == ERR_NONE)
        {
            Serial.print(F("[SX1276] Data:\t\t"));
            // Serial.print(str);
            for (byte i = 0; i < sizeOfData; i++)
            {
                byte2float(&Data[i], byteArr, i * 4);
            }
            Serial.print(F("[SX1276] RSSI:\t\t"));
            Serial.print(radio.getRSSI());
            Serial.println(F(" dBm"));
        }
        else if (state == ERR_CRC_MISMATCH)
        {
            // packet was received, but is malformed
            Serial.println(F("[SX1276] CRC error!"));
        }
        else
        {
            // some other error occurred
            Serial.print(F("[SX1276] Failed, code "));
            Serial.println(state);
        }

        // put module back to listen mode
        radio.startReceive();

        // we're ready to receive more packets,
        // enable interrupt service routine
        enableInterrupt = true;
    }
}

void SenderLoop(float *FL, byte sizeOfData)
{
    if (millis() - loraLoopMillis > loraInterval)
    {   
        
        byte size = sizeOfData * 4;
        byte buff[size];
        for (byte i = 0; i < sizeOfData; i++)
        {
            float2byte(FL[i], buff, i * 4);
        Serial.print(FL[i]);
        Serial.print(",");

        }
        Serial.println();
        int transmissionState = ERR_NONE;
        transmissionState = radio.transmit(buff, size);
        // check if the previous transmission finished
        if (receivedFlag)
        {
            // disable the interrupt service routine while
            // processing the data
            enableInterrupt = false;
            // reset flag
            receivedFlag = false;
            if (transmissionState == ERR_NONE)
            {
            }
            else
            {
                Serial.print(F("failed, code "));
                Serial.println(transmissionState);
            }
            enableInterrupt = true;
        }
        loraLoopMillis = millis();
        Serial.print(F("[SX1276] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
    }
}
