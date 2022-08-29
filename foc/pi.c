// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file pi.c
 *
 * @brief This module implements Proportional Integral Control (PI).
 *
 * Component: PI
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
#include "pi.h"

// </editor-fold>

/**
* <B> Function: MCAPP_PIInit(MCAPP_PI_T *)  </B>
*
* @brief Function to reset variables used for PI Control.
*        .
*
* @param Pointer to the data structure containing PI Controller parameters.
* @return none.
* @example
* <CODE> MCAPP_PIInit(&pi); </CODE>
*
*/
void MCAPP_PIInit(MCAPP_PI_T *pPIParam)
{
    pPIParam->qdSum = 0;
    pPIParam->qOut = 0;
}

/**
* <B> Function: MCAPP_PIController(MCAPP_PI_T *)  </B>
*
* @brief Function implementing PI Controller.
*        .
* @param Pointer to the data structure containing PI Controller parameters.
* @return none.
* @example
* <CODE> MCAPP_PIController(&pi); </CODE>
*
*/
void MCAPP_PIController( MCAPP_PI_T *pPIParam)
{
    int16_t outTemp, currentError;

    currentError = pPIParam->qInRef - pPIParam->qInMeas;
    pPIParam->qKpOut  = __builtin_mulss(currentError, pPIParam->qKp);
    if (pPIParam->qKpScaleInt > 0)
    {
        pPIParam->qKpOut  = pPIParam->qKpOut  << (int32_t)pPIParam->qKpScaleInt;
    }
    pPIParam->qdSumUnsat = pPIParam->qdSum + 
                    __builtin_mulss(currentError, pPIParam->qKi);
    
    pPIParam->qdUnSatOutSum = pPIParam->qKpOut + pPIParam->qdSumUnsat;
    
    pPIParam->qOutMax_32bit = ((int32_t)pPIParam->qOutMax) << (int32_t)15;
    pPIParam->qOutMin_32bit = ((int32_t)pPIParam->qOutMin) << (int32_t)15;
    
	if(pPIParam->qdUnSatOutSum >=  pPIParam->qOutMax_32bit)
    {        
		pPIParam->qOut =  pPIParam->qOutMax; 
        pPIParam->qdSumUnsat = pPIParam->qOut - pPIParam->qKpOut;
    }
	else if(pPIParam->qdUnSatOutSum <= pPIParam->qOutMin_32bit)
    {
		pPIParam->qOut =  pPIParam->qOutMin;
        pPIParam->qdSumUnsat = pPIParam->qOut - pPIParam->qKpOut;
    }
	else
    {
		pPIParam->qOut = (int16_t)(pPIParam->qdUnSatOutSum >> 15);
        pPIParam->qdSum = pPIParam->qdSumUnsat;
    }
}
