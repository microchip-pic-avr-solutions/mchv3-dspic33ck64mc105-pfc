// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file foc_control_types.h
 *
 * @brief This module holds variable type definitions of data structure holding
 * FOC control parameters and motor parameters.
 *
 * Component: FOC
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

#ifndef __FOC_CONTROL_TYPES_H
#define __FOC_CONTROL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    int16_t
        qVelRef,                    /* Reference Velocity */
        qIdRef,                     /* D axis Current Reference Value */
        qIqRef,                     /* Q axis Current Reference Value */
        CLSpeedRampRate,            /* Ramp Reference for Speed */
        qDiff,                      /* Difference Between Reference & 
                                    * Target Speed */
        speedRampSkipCnt,           /* Speed ramp update counter */
        speedRampSkipCntLimit,      /* Speed ramp slew rate in open loop */
        speedRampIncLimit,          /* Ramp up slew rate */
        speedRampDecLimit,          /* Ramp down slew rate */
        OLTheta,                    /* Open Loop Angle */
        lockCurrent,                /* Locking current */
        OLCurrent,                  /* Open Loop Current */
        OLCurrentMax,               /* Maximum Open Loop Current */
        OLCurrentRampSkipCnt,       /* Current ramp update counter */
        OLCurrentRampSkipCntLimit,  /* Current Ramp slew rate in open loop */
        OLCurrentRampRate,          /* Current Ramp rate in open loop */
        normDeltaT,                 /* Scaled sampling time */
        OmegaDtGain,                /* Scaling of inductive drop */
        qTargetVelocity;            /* Speed Reference */

    uint16_t
        openLoop,                   /* Open loop flag */
        lockTime,                   /* Lock variable for initial ramp */
        lockTimeLimit,              /* Total lock time */
        OLSpeedRampRate;            /* Ramp rate for Open loop */

    int32_t
        OLThetaSum;                 /* Open loop Theta Accumulation */

} MCAPP_CONTROL_T;

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* end of __FOC_CONTROL_TYPES_H */
