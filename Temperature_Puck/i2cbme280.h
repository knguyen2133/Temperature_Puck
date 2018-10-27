#ifndef I2CBME280_H_
#define I2CBME280_H_

void I2cBme280Init();
uint8_t CaptureAmbient(float *temp, uint32_t *pressure, uint32_t *humidity);

#endif /* I2CBME280_H_ */
