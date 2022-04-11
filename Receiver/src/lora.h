#include <RadioLib.h>
#include "utilities.h"
uint32_t loraLoopMillis = 0;

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
    int state = radio.begin(LoRa_frequency);
    // int state = radio.begin(137.0);
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
    // set the function that will be called
    // when new packet is received
#ifdef RADIO_USING_SX1262
    radio.setDio1Action(setFlag);
#else
    radio.setDio0Action(setFlag);
#endif

    // start listening for LoRa packets
#ifdef HAS_DISPLAY
    if (u8g2)
    {
        if (state != ERR_NONE)
        {
            u8g2->clearBuffer();
            u8g2->drawStr(0, 12, "Initializing: FAIL!");
            u8g2->sendBuffer();
        }
    }
#endif
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
void ReceiverLoop(float *Data, byte sizeOfData)
{
    // check if the flag is set
    if (receivedFlag)
    {
        // disable the interrupt service routine while
        // processing the data
        enableInterrupt = false;

        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        // String str;
        // int state = radio.readData(str);

        // you can also read received data as byte array
        byte byteArr[sizeOfData * 4];
        int state = radio.readData(byteArr, sizeOfData * 4);
        if (state == ERR_NONE)
        {
            // packet was successfully received
            // Serial.println(F("[SX1276] Received packet!"));

            // print data of the packet
            // Serial.print(F("[SX1276] Data:\t\t"));
            // // Serial.print(str);
            for (byte i = 0; i < sizeOfData; i++)
            {
                byte2float(&Data[i], byteArr, i * 4);
                // Serial.print(Data[i]);
                // Serial.print(",");
            }
            // // char buf[256];
            // //   terminal.println(str);
            // // terminal.flush();
            // //             snprintf(buf, sizeof(buf), "LoRa RSSI: %.2f dbm \t Wifi RSSI: %d dbm ",radio.getRSSI(),WiFi.RSSI());
            // // terminal.println(buf);
            // //             terminal.flush();

            // // print RSSI (Received Signal Strength Indicator)
            // Serial.print(F("[SX1276] RSSI:\t\t"));
            // Serial.print(radio.getRSSI());
            // Serial.println(F(" dBm"));
#ifdef SNR
            // print SNR (Signal-to-Noise Ratio)

            Serial.print(F("[SX1276] SNR:\t\t"));
            Serial.print(radio.getSNR());
            Serial.println(F(" dB"));

            // print frequency error
            Serial.print(F("[SX1276] Frequency error:\t"));
            Serial.print(radio.getFrequencyError());
            Serial.println(F(" Hz"));
#endif
#ifdef HAS_DISPLAY
            if (u8g2)
            {
                u8g2->clearBuffer();
                char buf[256];
                u8g2->drawStr(0, 12, "Received OK!");
                u8g2->drawStr(5, 26, str.c_str());
                snprintf(buf, sizeof(buf), "RSSI:%.2f", radio.getRSSI());
                u8g2->drawStr(0, 40, buf);
                snprintf(buf, sizeof(buf), "SNR:%.2f", radio.getSNR());
                u8g2->drawStr(0, 54, buf);
                u8g2->sendBuffer();
            }
#endif
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
    // snprintf(buff[0], sizeof(buff[0]), "%s %s", buff[1],buff[2]);
    // snprintf(buff, sizeof(buff), "%s", buff);
    // Send data every 3 seconds

    if (millis() - loraLoopMillis > 1000)
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
                // packet was successfully sent
                // Serial.println(F("transmission finished!"));
                // NOTE: when using interrupt-driven transmit method,
                //       it is not possible to automatically measure
                //       transmission data rate using getDataRate()
            }
            else
            {
                Serial.print(F("failed, code "));
                Serial.println(transmissionState);
            }
            // wait a second before transmitting again

            // send another one
            // Serial.print(F("[RADIO] Sending another packet ... "));

            // you can transmit C-string or Arduino string up to
            // 256 characters long

            // transmissionState = radio.transmit(String(loraLoopMillis).c_str());

            // you can also transmit byte array up to 256 bytes long
            /*
              byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                                0x89, 0xAB, 0xCD, 0xEF};
              int state = radio.startTransmit(byteArr, 8);
            */

            // we're ready to send more packets,
            // enable interrupt service routine
            enableInterrupt = true;
        }
        loraLoopMillis = millis();
    }
}
