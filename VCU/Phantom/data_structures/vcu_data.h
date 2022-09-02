/*
 * vcu_data.h
 *
 *  Created on: Apr 18, 2020
 *      Author: soroush, Josh Guo
 */

#ifndef PHANTOM_DATA_STRUCTURES_VCU_DATA_H_
#define PHANTOM_DATA_STRUCTURES_VCU_DATA_H_

#include "hal_stdtypes.h"
#include "vcu_common.h"

typedef enum {

    // Shutdown Circuit Faults
    IMD_SEVERE_FAULT = (1U),
    BSPD_SEVERE_FAULT = (1U << 1),
    BMS_GPIO_SEVERE_FAULT = (1U << 2),                          // Changed BMS_FAULT to BMS_GPIO_FAULT - jjkhan
    
    // APPS/BSE Faults
    BSE_RANGE_SEVERE_FAULT = (1U << 3),
    APPS1_RANGE_SEVERE_FAULT = (1U << 4),
    APPS2_RANGE_SEVERE_FAULT = (1U << 5),
    APPS_10DIFF_SEVERE_FAULT = (1U << 6),
    BSE_APPS_SIMULTANEOUS_MINOR_FAULT = (1U << 7),
    
    // HV Sensor Faults
    HV_CURRENT_OUT_OF_RANGE_MINOR_FAULT = (1U << 8),            // SEVERE if persistent
    HV_VOLTAGE_OUT_OF_RANGE_MINOR_FAULT = (1U << 9),            // SEVERE if persistent
    HV_APPS_PROPORTION_SEVERE_ERROR = (1U << 10),                // Stays in TRACTIVE_OFF if already in TRACTIVE_OFF state

    // LV Sensor Faults
    LV_CURRENT_OUT_OF_RANGE_MINOR_FAULT = (1U << 11),
    LV_VOLTAGE_OUT_OF_RANGE_MINOR_FAULT = (1U << 12),

    // CAN Faults
    CAN_ERROR_TYPE1 = (1U << 13),                                // Severe Error reported by CAN
    CAN_ERROR_TYPE2 = (1U << 14),                                // Minor Errot Reported by CAN

    // TSAL Faults
    TSAL_WELDED_AIRS_SEVERE_FAULT = (1U << 15),

    // IMD Faults
    IMD_LOW_ISO_SEVERE_FAULT = (1U << 16),
    IMD_SHORT_CIRCUIT_SEVERE_FAULT = (1U << 17),
    IMD_DEVICE_ERR_SEVERE_FAULT = (1U << 18),
    IMD_BAD_INFO_SEVERE_FAULT = (1U << 19),
    IMD_UNDEF_SEVERE_FAULT = (1U << 20),
    IMD_GARBAGE_DATA_SEVERE_FAULT = (1U << 21),

    ALL_FAULTS = (~0U)                              // All 1s to let all faults through the mask
} Fault;

typedef enum {
    SHUTDOWN_CIRCUIT_FAULT_GROUP = IMD_SEVERE_FAULT || BSPD_SEVERE_FAULT || BMS_GPIO_SEVERE_FAULT,
    BSE_APPS_FAULT_GROUP = BSE_RANGE_SEVERE_FAULT || APPS1_RANGE_SEVERE_FAULT || APPS2_RANGE_SEVERE_FAULT || APPS_10DIFF_SEVERE_FAULT || BSE_APPS_SIMULTANEOUS_MINOR_FAULT,
    HV_FAULT_GROUP = HV_CURRENT_OUT_OF_RANGE_MINOR_FAULT || HV_VOLTAGE_OUT_OF_RANGE_MINOR_FAULT || HV_APPS_PROPORTION_SEVERE_ERROR,
    LV_FAULT_GROUP = LV_CURRENT_OUT_OF_RANGE_MINOR_FAULT || LV_VOLTAGE_OUT_OF_RANGE_MINOR_FAULT,
    CAN_FAULT_GROUP = CAN_ERROR_TYPE1 || CAN_ERROR_TYPE2,
    TSAL_FAULT_GROUP = TSAL_WELDED_AIRS_SEVERE_FAULT,
    IMD_FAULT_GROUP = IMD_LOW_ISO_SEVERE_FAULT || IMD_SHORT_CIRCUIT_SEVERE_FAULT || IMD_DEVICE_ERR_SEVERE_FAULT || IMD_BAD_INFO_SEVERE_FAULT || IMD_UNDEF_SEVERE_FAULT || IMD_GARBAGE_DATA_SEVERE_FAULT
} FaultGroup;

// function prototypes
void VCUData_init(void);

uint32 VCUData_readFaults(uint32 mask);
bool VCUData_turnOnFaults(uint32 mask);
bool VCUData_turnOffFaults(uint32 mask);
bool VCUData_setFaults(uint32 mask);


bool VCUData_getTSALSignal(void);
bool VCUData_getRTDSignal(void);
bool VCUData_getBrakeLightSignal(void);
bool VCUData_getThrottleAvailableSignal(void);
State VCUData_getState(void);

bool VCUData_setTSALSignal(bool newSignal);
bool VCUData_setRTDSignal(bool newSignal);
bool VCUData_setBrakeLightSignal(bool newSignal);
bool VCUData_setThrottleAvailableSignal(bool newSignal);
bool VCUData_setState(State newState);


float VCUData_getCurrentHV_A(void);
float VCUData_getVoltageHV_V(void);
float VCUData_getCurrentLV_A(void);
float VCUData_getVoltageLV_V(void);
float VCUData_getBSEPercentage(void);
float VCUData_getAPPS1Percentage(void);
float VCUData_getAPPS2Percentage(void);
float VCUData_getThrottlePercentage(void);

bool VCUData_setCurrentHV_A(float newValue);
bool VCUData_setVoltageHV_V(float newValue);
bool VCUData_setCurrentLV_A(float newValue);
bool VCUData_setVoltageLV_V(float newValue);
bool VCUData_setBSEPercentage(float newValue);
bool VCUData_setAPPS1Percentage(float newValue);
bool VCUData_setAPPS2Percentage(float newValue);
bool VCUData_setThrottlePercentage(float newValue);

// Any module that needs direct access
// to the VCU data structure needs to 
// #define VCUDATA_PRIVLEDGED_ACCESS in their file
#ifdef VCUDATA_PRIVLEDGED_ACCESS

typedef struct {
    float currentHV_A;
    float voltageHV_V;
    float currentLV_A;
    float voltageLV_V;
    float BSE_percentage;
    float APPS1_percentage;
    float APPS2_percentage;
    
    float throttle_percentage;

    bool TSAL_signal;
    bool RTD_signal;
    bool throttle_available;
    
    uint32 fault_flags;

    bool brake_light_signal;

    State VCU_state;
} VCUData;

VCUData* VCUData_getPointer(void);

#endif

#endif /* PHANTOM_DATA_STRUCTURES_VCU_DATA_H_ */
