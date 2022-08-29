// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file stall_detect.c
 *
 * @brief This module implements Stall Detection.
 * This is based on back EMF as well as estimated speed.
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

/* _Q15abs and _Q15sqrt function use */
#include <libq.h>

#include "stall_detect.h"
#include "hal/port_config.h"

// </editor-fold>

/**
* <B> Function: int16_t MCAPP_StallDetectInit(MCAPP_STALL_DETECT_T *)  </B>
*
* @brief Function to Initialize stall detect Data Structure variables.
*
* @param    pointer to the data structure containing stall detection parameters.
* @return   status.
* @example
* <CODE> MCAPP_StallDetectInit(&stall_detect); </CODE>
*
*/

void MCAPP_StallDetectInit(MCAPP_STALL_DETECT_T *pStallDet)
{
    pStallDet->EmagStallCount = 0;
    pStallDet->qThetaRef = 0;
    pStallDet->qEdFiltStateVar = 0;
    pStallDet->qEqFiltStateVar = 0;
    pStallDet->status = 0;
}

/**
* <B> Function: void MCAPP_StallDetect(MCAPP_STALL_DETECT_T *)  </B>
*
* @brief Function to Initialize stall detect Data Structure variables.
*
* @param    pointer to the data structure containing stall detection parameters.
* @return   none.
* @example
* <CODE> MCAPP_StallDetect(&stall_detect); </CODE>
*
*/

int16_t MCAPP_StallDetect(MCAPP_STALL_DETECT_T *pStallDet)
{
    int16_t 
        tmp, 
        filtErr, 
        edSqr, 
        eqSqr;
    
    const MCAPP_CONTROL_T   *pCtrlParam = pStallDet->pCtrlParam;
    const MC_ALPHABETA_T *pEAlphaBeta = pStallDet->pEAlphaBeta;
    const MC_DQ_T *pidq = pStallDet->pidq;
    
    MC_SINCOS_T stallSinCos;
    
    /** BEMF based stall detection */
    
    pStallDet->qThetaRef += (int16_t)(__builtin_mulss(pCtrlParam->qVelRef, 
                                            pCtrlParam->normDeltaT) >> 15);
    MC_CalculateSineCosine_Assembly_Ram(pStallDet->qThetaRef, &stallSinCos);

    MC_TransformPark_Assembly(pEAlphaBeta, &stallSinCos, &pStallDet->qEdq);
    
    filtErr = pStallDet->qEdq.d - pStallDet->qEdqFilt.d;
    pStallDet->qEdFiltStateVar += 
                    __builtin_mulss(filtErr, pStallDet->qEmagFiltConst);
    pStallDet->qEdqFilt.d = (int16_t)(pStallDet->qEdFiltStateVar >> 15);
    
    filtErr = pStallDet->qEdq.q - pStallDet->qEdqFilt.q;
    pStallDet->qEqFiltStateVar += 
                    __builtin_mulss(filtErr, pStallDet->qEmagFiltConst);
    pStallDet->qEdqFilt.q = (int16_t)(pStallDet->qEqFiltStateVar >> 15);
    
    /* Calculate BEMF magnitude */
    edSqr = (int16_t)(__builtin_mulss(pStallDet->qEdqFilt.d,
                    pStallDet->qEdqFilt.d) >> 15);
    eqSqr = (int16_t)(__builtin_mulss(pStallDet->qEdqFilt.q,
                    pStallDet->qEdqFilt.q) >> 15);
    
    pStallDet->qEmagFilt = _Q15sqrt(edSqr + eqSqr);
    
    pStallDet->qEmagLimit = 
                (int16_t)(__builtin_mulss(*(pStallDet->pEmagExp),
                            pStallDet->qEmagMargin) >> 15);
    
    if(pStallDet->qEmagFilt <= pStallDet->qEmagLimit)
    {
        pStallDet->EmagStallCount++;
        if(pStallDet->EmagStallCount >= pStallDet->EmagStallCountLimit)
        {
//            LED2 = 1;
            pStallDet->status = 2;
        }
    }
    else
    {
        pStallDet->EmagStallCount = 0;
    }

    /** Current based Stall Detection */
    tmp = _Q15sqrt((__builtin_mulss(pidq->d, pidq->d) >> 15) + 
            (__builtin_mulss(pidq->q, pidq->q) >> 15));
    if(tmp >= pStallDet->currLimit)
    {
        pStallDet->currStallCount++;
        if(pStallDet->currStallCount >= pStallDet->currStallCountLimit)
        {
            pStallDet->status = 1;
        }
    }
    else
    {
        pStallDet->currStallCount = 0;
    }
    
    return pStallDet->status;
}