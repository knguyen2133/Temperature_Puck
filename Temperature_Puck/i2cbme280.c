/*
 * Copyright (c) 2016-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *    ======== testbutton.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* XDC Header files */
#include <xdc/runtime/System.h>

/* POSIX Header files */
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Module Header */
#include <ti/sail/bme280/bme280.h>

/* Example/Board Header files */
#include "Board.h"

I2C_Handle      i2c;
I2C_Params      i2cParams;

extern s32 bme280_data_readout_template(I2C_Handle i2cHndl);

void I2cBme280Init()
{
    /* Call driver init functions */
       GPIO_init();
       I2C_init();

    /* Open the HOST display for output */

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.transferCallbackFxn = NULL;
    i2c = I2C_open(Board_I2C0, &i2cParams);
    if (i2c == NULL)
    {
        System_printf("Error Initializing I2C\n");
    }
    else
    {
        System_printf("I2C Initialized!\n");
    }

    /* Initialize the BME Sensor */
    bme280_data_readout_template(i2c);
    bme280_set_power_mode(BME280_NORMAL_MODE);

}

uint8_t CaptureAmbient(float *temp, uint32_t *pressure, uint32_t *humidity)
{
    int32_t     actualTemp   = 0;
    uint8_t     retval = 0;
    retval = bme280_read_pressure_temperature_humidity(pressure, &actualTemp, humidity);

    /*Farenheit*/
    *temp = (actualTemp/(float)100) * (9/(float)5) + 32;

    /*System_printf("%d KPa, %f DegF, %d %%RH\n",
                  (*pressure)/1000, *temp,
                  (*humidity)/1000);
     */
    return retval;
}
