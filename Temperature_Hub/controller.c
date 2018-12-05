#include <stdint.h>
#include <stddef.h>

/* XDC Header files */
#include <xdc/runtime/System.h>

/* TI Header files */
#include <ti/sysbios/knl/Task.h>

#include "controller.h"
#include "../Radio_Common/packet.h"
#include "../Radio_Common/rx_radio.h"

void RunHub()
{
    uint8_t rxBuffer[MAX_LENGTH];

    while(1)
    {
        if(BlockingRx(rxBuffer))
        {
            DecodePacket(rxBuffer);
            //send to server
        }
        else
        {
            //System_printf("Unable to receive message");
        }
        Task_sleep(1);
    }
}


