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

int32_t     g_s32ActualTemp   = 0;
uint32_t    g_u32ActualPress  = 0;
uint32_t    g_u32ActualHumity = 0;

I2C_Handle      i2c;
I2C_Params      i2cParams;

static Display_Handle display;

extern s32 bme280_data_readout_template(I2C_Handle i2cHndl);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    float fDegree;
    /* Call driver init functions */
    GPIO_init();
    I2C_init();

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }
    Display_print0(display, 0, 0, "Starting the i2cbme280 sensor example...\n\n");

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.transferCallbackFxn = NULL;
    i2c = I2C_open(Board_I2C0, &i2cParams);
    if (i2c == NULL) {
        Display_print0(display, 0, 0, "Error Initializing I2C\n");
    }
    else {
        Display_print0(display, 0, 0, "I2C Initialized!\n");
    }

    /* Initialize the BME Sensor */
    bme280_data_readout_template(i2c);
    bme280_set_power_mode(BME280_NORMAL_MODE);

    while(1)
    {
        bme280_read_pressure_temperature_humidity(
        &g_u32ActualPress, &g_s32ActualTemp, &g_u32ActualHumity);

        fDegree = (g_s32ActualTemp/(float)100) * (9/(float)5) + 32;

        Display_print3(display, 0, 0, "%u KPa(Pressure), %f DegF(Temp), %u %%RH(Humidity)\n",
                    g_u32ActualPress/1000, fDegree,
                    g_u32ActualHumity/1000);

        sleep(1);
    }
}
