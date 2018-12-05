#ifndef PACKET_H_
#define PACKET_H_

#define MAX_LENGTH          255
/* 1b      8b   1b    4b    4b        4b        4b
 * [LENGTH][MAC][TYPE][TEMP][PRESSURE][HUMIDITY][NONCE]
 * */
#define AMBIENT_LENGTH      30

#define LENGTH_INDEX        0
#define MAC_INDEX           1
#define TYPE_INDEX          9
#define TEMP_INDEX          13
#define PRESSURE_INDEX      17
#define HUMIDITY_INDEX      21
#define NONCE_INDEX         25

void DecodePacket(uint8_t *rxBuffer);
void CreateTxPacket(uint8_t *txBuffer, uint8_t length, float temperature, uint32_t pressure, uint32_t humidity);

#endif /* PACKET_H_ */
