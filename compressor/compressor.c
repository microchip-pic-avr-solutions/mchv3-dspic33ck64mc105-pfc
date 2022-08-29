// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file compressor.c
 *
 * @brief This module implements compressor state machine.
 *
 * Component: COMPRESSOR LOAD
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

#include "compressor.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void MCAPP_CompressorStateMachine (MCAPP_COMPRESSOR_T *);
void MCAPP_CompressorInit (MCAPP_COMPRESSOR_T *);

// </editor-fold>
/**
* <B> Function: void MCAPP_CompressorInit (MCAPP_COMPRESSOR_T *)  </B>
*
* @brief Compressor load initialization.
*
* @param Pointer to the data structure containing compressor parameters.
* @return none.
* @example
* <CODE> MCAPP_CompressorInit(&compr); </CODE>
*
*/
void MCAPP_CompressorInit (MCAPP_COMPRESSOR_T *pComp)
{
    pComp->state = COMPRESSOR_WAIT;
    
    pComp->torqueComp.mechCount = 0;
    pComp->torqueComp.qOmegaAcc = 0;
}

/**
* <B> Function: void MCAPP_CompressorStateMachine (MCAPP_COMPRESSOR_T *)  </B>
*
* @brief Compressor state machine.
*
* @param Pointer to the data structure containing compressor parameters.
* @return none.
* @example
* <CODE> MCAPP_CompressorStateMachine(&compr); </CODE>
*
*/
void MCAPP_CompressorStateMachine (MCAPP_COMPRESSOR_T *pComp)
{
    switch(pComp->state)
    {
        case COMPRESSOR_WAIT:
            break;
            
        case COMPRESSOR_RUN:
            MCAPP_TorqueComp(&pComp->torqueComp);
            break;
            
        case COMPRESSOR_STOP:
            break;
            
        case COMPRESSOR_FAULT:            
            break;
            
        default:
            pComp->state = COMPRESSOR_FAULT;
        break;
    }
}

/**
* <B> Function: void MCAPP_IsCompressorReadyToStart (MCAPP_COMPRESSOR_T *) </B>
*
* @brief Compressor Ready to Start Interface Function.
*
* @param Pointer to the data structure containing compressor parameters.
* @return Ready to start Flag.
* @example
* <CODE> MCAPP_IsCompressorReadyToStart(&compr); </CODE>
*
*/
int16_t MCAPP_IsCompressorReadyToStart (MCAPP_COMPRESSOR_T *pComp)
{
    return 1;
}

/**
* <B> Function: void MCAPP_IsLoadReadyToStop (MCAPP_COMPRESSOR_T *)  </B>
*
* @brief Compressor Ready to Start Interface Function.
*
* @param Pointer to the data structure containing compressor parameters.
* @return Ready to stop Flag.
* @example
* <CODE> MCAPP_IsLoadReadyToStop(&compr); </CODE>
*
*/
int16_t MCAPP_IsCompressorReadyToStop (MCAPP_COMPRESSOR_T *pComp)
{
    return 1;
}