#include <stdint.h>
#include <stddef.h>

/* XDC Header files */
#include <xdc/runtime/System.h>

/* TI Header files */
#include <ti/sysbios/knl/Task.h>

#include "controller.h"
#include "../Radio_Common/packet.h"
#include "../Radio_Common/tx_radio.h"
#include "i2cbme280.h"

void RunPuck()
{
    uint8_t retval;
    float temperature;
    uint32_t pressure, humidity;
    uint8_t txBuffer[AMBIENT_LENGTH];

    while(1)
    {
        retval = CaptureAmbient(&temperature, &pressure, &humidity);
        if(retval == 0)
        {
            CreateTxPacket(txBuffer, AMBIENT_LENGTH, temperature, pressure, humidity);
            BlockingTx(txBuffer, AMBIENT_LENGTH);
        }
        else
        {
            System_printf("Unable to Capture Ambient");
        }

        Task_sleep(1);
    }
}


