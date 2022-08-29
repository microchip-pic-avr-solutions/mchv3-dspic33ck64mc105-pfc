// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_init.h
 *
 * @brief This module initializes data structure holding motor control
 * parameters required to run motor 1 using field oriented control.
 * In this application to initialize variable required to run Compressor.
 *
 * Component: APPLICATION (Motor Control 1 - mc1)
 *
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">

/*******************************************************************************
* Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
*
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
// </editor-fold>

#ifndef __MC1_INIT_H
#define __MC1_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include "measure.h"
#include "motor_params.h"
#include "foc.h"
#include "compressor.h"
    
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    int16_t
        appState,                   /* Application State */
        runCmd,                     /* Run command for motor */
        runCmdBuffer,               /* Run command buffer for validation */
        qTargetVelocity,            /* Target motor Velocity */
        qMaxSpeedFactor;            /* Maximum speed to peak speed ratio */
    
    MCAPP_MEASURE_T
        motorInputs;
    
    MCAPP_MOTOR_T
        motor;
    
    MCAPP_CONTROL_SCHEME_T
        controlScheme;              /* Motor Control parameters */
    
    MCAPP_LOAD_T
        load;                       /* Load parameters */
    
    MC_DUTYCYCLEOUT_T
        PWMDuty;
    
    MCAPP_MEASURE_T *pMotorInputs;
    MCAPP_MOTOR_T *pMotor;
    MCAPP_CONTROL_SCHEME_T *pControlScheme;
    MCAPP_LOAD_T *pLoad;
    MC_DUTYCYCLEOUT_T *pPWMDuty;
        
    /* Function pointers for motor inputs */    
    void (*MCAPP_InputsInit) (MCAPP_MEASURE_T *);
    void (*MCAPP_MeasureOffset) (MCAPP_MEASURE_T *);
    void (*MCAPP_GetProcessedInputs) (MCAPP_MEASURE_T *);
    int16_t (*MCAPP_IsOffsetMeasurementComplete) (MCAPP_MEASURE_T *);
    void (*HAL_MotorInputsRead) (MCAPP_MEASURE_T *);
    
    /* Function pointers for control scheme */
    void (*MCAPP_ControlSchemeInit) (MCAPP_CONTROL_SCHEME_T *);
    void (*MCAPP_ControlStateMachine) (MCAPP_CONTROL_SCHEME_T *);
    
    /* Function pointers for load */
    void (*MCAPP_LoadStateMachine) (MCAPP_LOAD_T *);
    void (*MCAPP_LoadInit) (MCAPP_LOAD_T *);
    
    void (*MCAPP_LoadStartTransition) (MCAPP_CONTROL_SCHEME_T *, 
                                        MCAPP_LOAD_T *);
    void (*MCAPP_LoadStopTransition) (MCAPP_CONTROL_SCHEME_T *, 
                                        MCAPP_LOAD_T *);
    
    int16_t (*MCAPP_IsLoadReadyToStart) (MCAPP_LOAD_T *);
    int16_t (*MCAPP_IsLoadReadyToStop) (MCAPP_LOAD_T *);
    
    /* Function pointers for motor outputs */
    void (*HAL_PWMSetDutyCycles)(MC_DUTYCYCLEOUT_T *);
    void (*HAL_PWMEnableOutputs) (void);
    void (*HAL_PWMDisableOutputs) (void);
    void (*MCAPP_HALSetVoltageVector) (int16_t);

}MC1APP_DATA_T;

// </editor-fold>
    
// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void MCAPP_MC1ParamsInit(MC1APP_DATA_T *);

// </editor-fold>


#ifdef __cplusplus
}
#endif

#endif /* end of __MC1_INIT_H */
