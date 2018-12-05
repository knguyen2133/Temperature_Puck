#include <stdint.h>

#include <ti/drivers/rf/RF.h>
#include "RFQueue.h"

#include <driverlib/rf_data_entry.h>
#include <driverlib/rf_prop_mailbox.h>
#include <driverlib/rf_prop_cmd.h>

#include "smartrf_settings/smartrf_settings.h"

#include "packet.h"
#include "rx_radio.h"

#define RF_convertMsToRatTicks(milliseconds) \
    ((uint32_t)(milliseconds) * 4 * 1000)

#define NUM_DATA_ENTRIES       2  /* NOTE: Only two data entries supported at the moment */
#define NUM_APPENDED_BYTES     2  /* The Data Entries data field will contain:
                                   * 1 Header byte (RF_cmdPropRx.rxConf.bIncludeHdr = 0x1)
                                   * Max 30 payload bytes
                                   * 1 status byte (RF_cmdPropRx.rxConf.bAppendStatus = 0x1) */

RF_Params rfParams;
static RF_Object rfObject;
static RF_Handle rfHandle;
static dataQueue_t dataQueue;
static rfc_dataEntryGeneral_t* currentDataEntry;

static uint8_t packet[MAX_LENGTH + NUM_APPENDED_BYTES - 1]; /* The length byte is stored in a separate variable */
static uint8_t packetLength;
static uint8_t* packetDataPointer;

#pragma DATA_ALIGN (rxDataEntryBuffer, 4);
   static uint8_t rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                                    MAX_LENGTH,
                                                                    NUM_APPENDED_BYTES)];

uint8_t BlockingRx(uint8_t *rxBuffer)
{
    memset(packet, 0, MAX_LENGTH + NUM_APPENDED_BYTES - 1);

    RF_EventMask result=RF_runCmd(rfHandle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal,NULL, IRQ_RX_ENTRY_DONE);

    if (((RF_Op*)&RF_cmdPropRx)->status == PROP_DONE_RXTIMEOUT)
    {
        return 0;
    }

    if(result & RF_EventRxEmpty)
    {
        return 0;
    }

    /* Get current unhandled data entry */
    currentDataEntry = RFQueue_getDataEntry();

    /* Handle the packet data, located at &currentDataEntry->data:
     * - Length is the first byte with the current configuration
     * - Data starts from the second byte */
    packetLength      = *(uint8_t*)(&currentDataEntry->data);
    packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);


    /* Copy the payload + the status byte to the packet variable */
    memcpy(packet, packetDataPointer, (packetLength + 1));
    RFQueue_nextEntry();

//    uint8_t status=IsLocalMac(packet);
//    if(status != TRUE)
//    {
//        return FALSE;
//    }

    uint8_t length=packet[0];


    memcpy(rxBuffer, packet, length);

    return 1;
}

void SetupRx()
{
    RF_Params_init(&rfParams);

    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;  /* Discard ignored packets from Rx queue */
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = 1;   /* Discard packets with CRC error from Rx queue */
    RF_cmdPropRx.pktConf.bRepeatOk = 1;
    RF_cmdPropRx.pktConf.bRepeatNok = 1;
    RF_cmdPropRx.endTime = RF_convertMsToRatTicks(2000);
    RF_cmdPropRx.endTrigger.triggerType = 0x4;
    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &rfParams);

    /* Set the frequency */
    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    if( RFQueue_defineQueue(&dataQueue,
                                rxDataEntryBuffer,
                                sizeof(rxDataEntryBuffer),
                                NUM_DATA_ENTRIES,
                                MAX_LENGTH + NUM_APPENDED_BYTES)){
        //System_printf("RX ERROR\n");
    }

    /* Modify CMD_PROP_RX command for application needs */
    RF_cmdPropRx.pQueue = &dataQueue;           /* Set the Data Entity queue for received data */

    RF_cmdPropRx.maxPktLen = MAX_LENGTH;     /* Implement packet length filtering to avoid PROP_ERROR_RXBUF */

}




