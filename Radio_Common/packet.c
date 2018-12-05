#include <stdint.h>
#include <stdio.h>

#include <xdc/runtime/System.h>

#include "packet.h"

void PrintPacket(uint8_t *buffer, uint8_t buflen)
{
    uint8_t counter;
    for(counter = 0; counter < buflen; counter++)
    {
        System_printf("%02x ", buffer[counter]);
    }
    System_printf("\n");
}

void DecodePacket(uint8_t *rxBuffer)
{
    uint8_t length;
    uint64_t macAddr;
    uint8_t type;
    float temperature;
    uint32_t pressure;
    uint32_t humidity;

    length = rxBuffer[LENGTH_INDEX];
    memcpy(&macAddr, (uint64_t *) &rxBuffer[MAC_INDEX], 8);
    type = rxBuffer[TYPE_INDEX];
    memcpy(&temperature, (uint32_t *) &rxBuffer[TEMP_INDEX], 4);
    memcpy(&pressure, (uint32_t *) &rxBuffer[PRESSURE_INDEX], 4);
    memcpy(&humidity, (uint32_t *) &rxBuffer[HUMIDITY_INDEX], 4);

    PrintPacket(rxBuffer, length);
}
void CreateTxPacket(uint8_t *txBuffer, uint8_t length, float temperature, uint32_t pressure, uint32_t humidity)
{
    uint64_t *macAddr = (uint64_t *)(0x500012F0);

    txBuffer[LENGTH_INDEX] = length;
    memcpy(&txBuffer[MAC_INDEX], macAddr,  8);
    txBuffer[TYPE_INDEX] = 1;
    memcpy(&txBuffer[TEMP_INDEX], (uint8_t*)&temperature, 4);
    memcpy(&txBuffer[PRESSURE_INDEX], (uint8_t*)&pressure, 4);
    memcpy(&txBuffer[HUMIDITY_INDEX], (uint8_t*)&humidity, 4);

    PrintPacket(txBuffer, length);
}
