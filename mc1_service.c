// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_service.c
 *
 * @brief This module implements motor control.
 *
 * Component: MOTOR CONTROL APPLICATION
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include <libq.h>

#include "board_service.h"
#include "mc1_init.h"
#include "mc_app_types.h"
#include "mc1_service.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="VARIABLES ">

MC1APP_DATA_T mc1;
MC1APP_DATA_T *pMC1Data = &mc1;

extern int16_t PFC_Sample_Counter;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">

static void MC1APP_StateMachine(MC1APP_DATA_T *);
static void MCAPP_MC1ReceivedDataProcess(MC1APP_DATA_T *);

// </editor-fold>

/**
* <B> Function: void MC1APP_StateMachine (MC1APP_DATA_T *)  </B>
*
* @brief Application state machine.
*
* @param Pointer to the data structure containing Application parameters.
* @return none.
* @example
* <CODE> MC1APP_StateMachine(&mc); </CODE>
*
*/
static void MC1APP_StateMachine(MC1APP_DATA_T *pMCData)
{
    MCAPP_MEASURE_T *pMotorInputs = pMCData->pMotorInputs;
    MCAPP_CONTROL_SCHEME_T *pControlScheme = pMCData->pControlScheme;
    MCAPP_LOAD_T *pLoad = pMCData->pLoad;
    MCAPP_MOTOR_T *pMotor = pMCData->pMotor;
    
    switch(pMCData->appState)
    {
    case MCAPP_INIT:

        pMCData->HAL_PWMDisableOutputs();

        /* Stop the motor */
        pMCData->runCmd = 0;
        
        pMCData->MCAPP_ControlSchemeInit(pControlScheme);
        pMCData->MCAPP_InputsInit(pMotorInputs);
        pMCData->MCAPP_LoadInit(pLoad);       
        
        pMCData->appState = MCAPP_CMD_WAIT;

        break;
        
    case MCAPP_CMD_WAIT:
        if(pMCData->runCmd == 1)
        {
            pMCData->appState = MCAPP_OFFSET;
        }
       break;
       
    case MCAPP_OFFSET:

        /* Measure Initial Offsets */
        pMCData->MCAPP_MeasureOffset(pMotorInputs);

        if(pMCData->MCAPP_IsOffsetMeasurementComplete(pMotorInputs))
        {
            pMCData->appState = MCAPP_LOAD_START_READY_CHECK;
        }

        break;

    case MCAPP_LOAD_START_READY_CHECK:
        
        pMCData->MCAPP_GetProcessedInputs(pMotorInputs);
        pMCData->MCAPP_LoadStateMachine(pLoad);
        
        if(pMCData->MCAPP_IsLoadReadyToStart(pLoad))
        {
            /* Load is ready, start the motor */
            pMCData->HAL_PWMEnableOutputs();

            pMCData->MCAPP_LoadStartTransition(pControlScheme, pLoad);

            pMCData->appState = MCAPP_RUN;
        }
        break;
            
    case MCAPP_RUN:
        
        /* Compensate motor current offsets */
        pMCData->MCAPP_GetProcessedInputs(pMotorInputs);

        pMCData->MCAPP_ControlStateMachine(pControlScheme);
        
        pMCData->MCAPP_LoadStateMachine(pLoad);
        
        if (pMCData->runCmd == 0)
        {
            /* Set Target velocity to Open Loop Electrical Speed */
            pControlScheme->ctrlParam.qTargetVelocity = 
                                pMotor->qMaxOLSpeed;

            /* exit loop if motor not run */
            pMCData->appState = MCAPP_LOAD_STOP_READY_CHECK;
        }
        break;

    case MCAPP_LOAD_STOP_READY_CHECK:
        
        pMCData->MCAPP_LoadStateMachine(pLoad);
        
        /* Load is ready, stop the motor */
        pMCData->MCAPP_GetProcessedInputs(pMotorInputs);
        pMCData->MCAPP_ControlStateMachine(pControlScheme);
        
        if(pMCData->MCAPP_IsLoadReadyToStop(pLoad))
        {    
            pMCData->MCAPP_LoadStopTransition(pControlScheme, pLoad);
            pMCData->appState = MCAPP_STOP;
        }

        break;

    case MCAPP_STOP:
        pMCData->HAL_PWMDisableOutputs();
        pMCData->appState = MCAPP_INIT;

        break;
        
    case MCAPP_FAULT:
            
        break;
        
    default:
        pMCData->appState = MCAPP_FAULT;
        break;

    } /* end of switch-case */
    

//    if(pControlScheme->faultStatus)
//    {
//        pMCData->HAL_PWMDisableOutputs();
//        pMCData->appState = MCAPP_FAULT;
//    }
}

/**
* <B> Function: MC1_ADC_INTERRUPT()  </B>
*
* @brief ADC interrupt vector ,and it performs following actions:
*        (1) Reads DC BUS voltage and updates appropriate variables.
*        (2) Reads motor 1 phase currents,bus current and phase voltage
*            feedbacks from ADC data buffers.
*        (3) Executes Field Oriented Control based on the current,voltage
*            feedbacks.
*        (4) Loads duty cycle values generated by FOC to the registers
*            of PWM Generators controlling motor 1.
*/
void __attribute__((__interrupt__,no_auto_psv)) MC1_ADC_INTERRUPT()
{
    int16_t adcBuffer;
    
    pMC1Data->HAL_MotorInputsRead(pMC1Data->pMotorInputs);
    
    PFC_Sample_Counter = 0;
 //   HAL_MCADCSwitchChannels();
    
    MC1APP_StateMachine(pMC1Data);

    pMC1Data->HAL_PWMSetDutyCycles(pMC1Data->pPWMDuty);
    
    adcBuffer = MC1_ClearADCIF_ReadADCBUF();
	MC1_ClearADCIF();
    
}

void MCAPP_MC1ServiceInit(void)
{
    MCAPP_MC1ParamsInit(pMC1Data);
}

void MCAPP_MC1InputBufferSet(int16_t runCmd, 
                                    int16_t qTargetVelocity)
{
    MC1APP_DATA_T   *pMCData = pMC1Data;
    MCAPP_MOTOR_T   *pMotor = pMC1Data->pMotor;
    
    pMCData->qTargetVelocity =  pMotor->qMinSpeed + 
            (int16_t)(__builtin_mulss((pMotor->qMaxSpeed - 
            pMotor->qMinSpeed), qTargetVelocity) >> 15);
    
    pMCData->runCmdBuffer = runCmd;
    MCAPP_MC1ReceivedDataProcess(pMCData);
}

int16_t MCAPP_MC1GetTargetVelocity(void)
{
    return pMC1Data->motorInputs.measurePot;
}

static void MCAPP_MC1ReceivedDataProcess(MC1APP_DATA_T *pMCData)
{
    MCAPP_CONTROL_SCHEME_T *pControlScheme = pMCData->pControlScheme;
    MCAPP_MOTOR_T *pMotor = pMCData->pMotor;
    MCAPP_MEASURE_T *pMotorInputs = pMCData->pMotorInputs;

    if(pMCData->runCmd == 1)
    {
        if(pMCData->qTargetVelocity > pMotor->qMaxSpeed)
        {
            pMCData->qTargetVelocity = pMotor->qMaxSpeed;
        }
        else
        {
            if(pMCData->qTargetVelocity < pMotor->qMinSpeed)
            {
                pMCData->qTargetVelocity = pMotor->qMinSpeed;
            }
        }

        pControlScheme->ctrlParam.qTargetVelocity = pMCData->qTargetVelocity;
    }
    
//    if((pMCData->runCmdBuffer == 1) || (pMCData->runCmdBuffer == 0))
//    {
//        if((pMotorInputs->measureVdc.value >= 
//                pMotorInputs->measureVdc.dcMinRun) && 
//                (pControlScheme->faultStatus == 0))
//        {
//            pMCData->runCmd = pMCData->runCmdBuffer;
//        }        
//    }
    pMCData->runCmd = pMCData->runCmdBuffer;
    if(pMotorInputs->measureVdc.value < pMotorInputs->measureVdc.dcMaxStop)
    {
        pMCData->runCmd = 0;
        pMCData->controlScheme.faultStatus = 0;
    }
}
