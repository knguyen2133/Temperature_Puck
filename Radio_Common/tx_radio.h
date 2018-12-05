#ifndef TX_RADIO_H_
#define TX_RADIO_H_

void BlockingTx(uint8_t *buffer, uint8_t buflen);
void SetupTx();
void CloseTx();

#endif /* TX_RADIO_H_ */

