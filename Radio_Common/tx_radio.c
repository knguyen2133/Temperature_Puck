/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/rf/RF.h>
/* Board Header files */
#include "Board.h"
/* EasyLink API Header files */

#include "smartrf_settings/smartrf_settings.h"
#include "application_settings.h"
#include <stdio.h>
#include "tx_radio.h"
#include "packet.h"

RF_ScheduleCmdParams rfScheduleParams;
RF_Params txParams;
static RF_Object rfObject;
static RF_Handle rfHandle;
static uint32_t time;

#define CS_RETRIES_WHEN_BUSY    10      /* Number of times the CS command should run in the case where the channel is BUSY */
#define IDLE_TIME_US            5000
#define RSSI_THRESHOLD_DBM      -90     /* The channel is reported BUSY is the RSSI is above this threshold */
#define PACKET_INTERVAL_US      200000 //2 seconds


void BlockingTx(uint8_t *buffer, uint8_t buflen){
    RF_cmdPropTx.pktLen = buflen;
    RF_cmdPropTx.pPkt = buffer;

    /* Set absolute TX time to utilize automatic power management */
    time = RF_getCurrentTime();
    time += (PACKET_INTERVAL_US * 4);
    RF_cmdNop.startTime = time;

    RF_EventMask result = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdNop, RF_PriorityNormal, NULL, 0);

    if(result & RF_EventTxDone)
    {
        System_printf("Tx Successful\n");
    }

    RF_cmdNop.status = IDLE;
    RF_cmdPropCs.status = IDLE;
    RF_cmdCountBranch.status = IDLE;
    RF_cmdPropTx.status = IDLE;
    RF_cmdCountBranch.counter = CS_RETRIES_WHEN_BUSY;

}

void SetupTx()
{

    RF_Params_init(&txParams);

    RF_cmdNop.startTrigger.triggerType = TRIG_ABSTIME;
    RF_cmdNop.startTrigger.pastTrig = 1;

    /* Set up the next pointers for the command chain */
    RF_cmdNop.pNextOp = (rfc_radioOp_t*)&RF_cmdPropCs;
    RF_cmdPropCs.pNextOp = (rfc_radioOp_t*)&RF_cmdCountBranch;
    RF_cmdCountBranch.pNextOp = (rfc_radioOp_t*)&RF_cmdPropTx;
    RF_cmdCountBranch.pNextOpIfOk = (rfc_radioOp_t*)&RF_cmdPropCs;

    /* Customize the API commands with application specific defines */
    RF_cmdPropCs.rssiThr = RSSI_THRESHOLD_DBM;
    RF_cmdPropCs.csEndTime = (IDLE_TIME_US + 150) * 4; /* Add some margin */
    RF_cmdCountBranch.counter = CS_RETRIES_WHEN_BUSY;

    rfHandle = RF_open(&rfObject, &RF_prop,
                       (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, &txParams);

    RF_postCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

}



