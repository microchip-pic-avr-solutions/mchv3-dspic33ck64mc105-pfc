// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file stall_detect.h
 *
 * @brief This module implements Stall Detection.
 *
 * Component: STALL DETECTION
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

#ifndef STALL_DETECT_H
#define	STALL_DETECT_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include "estim_atpll.h"
#include "estim_interface.h"
#include "motor_control.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS">

typedef struct
{
    uint16_t 
        enable,                 /* Enable flag for stall detection */
        status;                   /* Stall detection flag. Set => stall */
    
    int16_t
        qThetaRef,              /* Reference angle */
        qEmagLimit,             /* BEMF limit */
        qEmagMargin,            /* Margin on expected BEMF */
        qEmagFiltConst,         /* BEMF filter constant */
        qEmagFilt,              /* Filtered BEMF magnitude */
        EmagStallCount,         /* BEMF based stall detection counter */
        EmagStallCountLimit,    /* BEMF stall counter persistence limit */
        currLimit,              /* Current limit for current 
                                 * based stall detection */
        currStallCount,         /* Current based stall detection counter */
        currStallCountLimit;    /* Current stall counter persistence limit */
    
    MC_DQ_T
        qEdq,                   /* BEMF in DQ domain */
        qEdqFilt;               /* Filtered BEMF in DQ domain */
    
    int32_t
        qEdFiltStateVar,    /* Ed filter variable */
        qEqFiltStateVar;    /* Eq filter variable */
    
    const int16_t *pEmagExp;
    const MC_ALPHABETA_T  *pEAlphaBeta;
    const MCAPP_CONTROL_T *pCtrlParam;
    const MC_DQ_T *pidq;
} MCAPP_STALL_DETECT_T;

// </editor-fold>

void MCAPP_StallDetectInit(MCAPP_STALL_DETECT_T *);
int16_t MCAPP_StallDetect(MCAPP_STALL_DETECT_T *);

#ifdef	__cplusplus
}
#endif

#endif	/* STALL_DETECT_H */

