/*
 * task_throttle.c
 *
 *  Created on: Mar 31, 2020
 *      Author: gabriel
 */

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "gio.h"
#include "os_queue.h"
#include "os_semphr.h"
#include "os_task.h"
#include "os_timer.h"

#include "board_hardware.h"   // contains hardware defines for specific board used (i.e. VCU or launchpad)

#include "Phantom_sci.h"
#include "LV_monitor.h"
#include "IMD.h"
#include "vcu_data.h"
#include "FreeRTOS.h"
#include "priorities.h"

// ++ Added by jjkhan

#include  "../execution_timer.h"

#define CPU_CLOCK_MHz (float) 160.0

volatile unsigned long cycles_PMU_start; // CPU cycle count at start
volatile float time_PMU_code_uSecond; // the calculated time in uSecond.

// -- Added by jjkhan


// change to better data type
int lv_current = 0;
// will also need one for lv_voltage; we should probably keep this in the vcu data structure?

/*********************************************************************************
 *                               SYSTEM STATE FLAGS
 *********************************************************************************/

extern data* VCUDataPtr;

extern uint8_t RTDS;// = 0;
long RTDS_RAW;      // = 0;
/***********************************************************
 * @function                - vSensorReadTask
 *
 * @brief                   - This task will read all the sensors in the vehicle (except for the APPS which requires more critical response)
 *
 * @param[in]               - pvParameters
 *
 * @return                  - None
 * @Note                    - None
 ***********************************************************/
void vSensorReadTask(void *pvParameters){

    // any initialization
    TickType_t xLastWakeTime;          // will hold the timestamp at which the task was last unblocked
    //const TickType_t xFrequency = 100; // task frequency in ms

    // Initialize the xLastWakeTime variable with the current time;
    xLastWakeTime = xTaskGetTickCount();
#ifdef RUN_TIME_STATS_SENSOR_READ
        /* Buffer to trace informations */
       static char cTraceBuffer[300];
       // -- Added by jjkhan For Storing RUN Time stats
#endif
    while(true)
    {
#ifdef PMU_CYCLE
       // Start timer.
       cycles_PMU_start = timer_Start();
       gioToggleBit(gioPORTA, 5);

#endif
        // Wait for the next cycle
        vTaskDelayUntil(&xLastWakeTime, SENSOR_READ_TASK_PERIOD_MS);

        // for timing:
        gioSetBit(hetPORT1, 25, 1);

//        MCP48FV_Set_Value(100);

//        gioToggleBit(gioPORTA, 5);

        // use getter function to get this value..
        // pass this data via a queue

        RTDS_RAW = gioGetBit(READY_TO_DRIVE_PORT, READY_TO_DRIVE_PIN);

        if ( gioGetBit(gioPORTA, 2) == 1)
        {
            VCUDataPtr->DigitalVal.RTDS = 0;
//            UARTSend(PC_UART, "RTDS RAW IS READ AS 1, RESETTING RTDS SIGNAL\r\n");
        }
        else
        {
//            UARTSend(PC_UART, "RTDS RAW IS READ AS 0, RESETTING RTDS SIGNAL\r\n");
        }

        if (TASK_PRINT) {UARTSend(PC_UART, "SENSOR READING TASK\r\n");}
//        UARTSend(scilinREG, xTaskGetTickCount());
        // read high voltage

        // read HV current

        // IMD data (maybe this needs to be a separate interrupt?)
        updateIMDData();
        serialSendData();

        // Shutdown GPIOs (will probably start with these non-interrupt and see if we need to later..)

        // TSAL state

        // CAN status from BMS (this may need an interrupt for when data arrives, and maybe stored in a buffer? maybe not.. we should try both)

        // read LV voltage, current

        // this needs to be updated to not block the whole system if i2c not available
//        lv_current = LV_reading(LV_current_register);

        // make sure state machine signal flags are updated

        // check for all errors here and update VCU data structure or state machine flags accordingly

        // will also need a lookup table or data structure that has error messages and LED codes for whatever fault flags are on

#ifdef PMU_CYCLE
    //gioToggleBit(gioPORTA, 5);
    //gioSetBit(gioPORTA, 5, 0);
    time_PMU_code_uSecond = timer_Stop(cycles_PMU_start, CPU_CLOCK_MHz);
    gioToggleBit(gioPORTA, 5);
#endif
        // for timing:
       gioSetBit(hetPORT1, 25, 0);
        // ++ Added by jjkhan for task profiling
#ifdef RUN_TIME_STATS_SENSOR_READ
         vTaskGetRunTimeStats(cTraceBuffer);
         printf(cTraceBuffer);
#endif
         // -- Added by jjkhan for task profiling
    }
}
