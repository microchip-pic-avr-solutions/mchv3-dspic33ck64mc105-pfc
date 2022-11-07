// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * pfc.h
 *
 * This is main header file to be included if  PFC functionality to be 
 * implemented
 * 

 * 
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

#ifndef __PFC_H
#define __PFC_H
    
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "pfc_general.h"
#include "pfc_pi.h"

#include "pfc_userparams.h"
#include "pfc_measure.h"

 
#ifdef	__cplusplus
extern "C" {
#endif
     
typedef struct
{
    uint16_t duty;
    uint16_t samplePoint;
    int16_t averageCurrent;
    int16_t rampRate;
    int16_t voltLoopExeRate;
}PFC_GENERAL_T;

typedef struct
{
    int32_t sum;
    int16_t output;
    int16_t scaler;
    int16_t samples;
    int16_t sampleLimit;
}PFC_AVG_T;
typedef struct
{
    int16_t sqrOutput;
    int32_t sum;
    int16_t samples;
    int16_t sampleLimit;
    int16_t peak;
    int16_t peakcheck;
}PFC_RMS_SQUARE_T;

typedef enum
{
    PFC_INIT = 0,
    PFC_OFFSET_MEAS = 1,
    PFC_CTRL_RUN = 2,
    PFC_FAULT = 3,       
}PFC_CTRL_STATE_T;
typedef enum
{
    PFC_FAULT_NONE = 0,
    PFC_FAULT_IP_UV = 1,
    PFC_FAULT_IP_OV = 2,
    PFC_FAULT_OP_OV = 4,
            
}PFC_FAULT_TYPE_T;
    
extern PFC_MEAS_T pfcMeasured;
extern PFC_AVG_T pfcVdcAVG;
extern PFC_RMS_SQUARE_T pfcVacRMS;

extern PFC_GENERAL_T pfcParam;

extern PFC_PI_T pfcPIVoltage;
extern PFC_PI_T pfcPICurrent;

extern PFC_CTRL_STATE_T pfcState;

extern void PFC_ControlLoopMain(void);
extern void PFC_Initialize(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PFC_H */

