// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file torque_comp.c
 *
 * @brief This module implements torque compensation feature.
 *
 * Component: TORQUE COMPENSATION
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

#include "torque_comp.h"

// </editor-fold>

/**
* <B> Function: MCAPP_TorqueCompInit(MCAPP_TORQUE_COMP_T *)  </B>
*
* @brief Function to reset variables used by the Torque Compensator function.
*
*
* @param Pointer to the data structure used by Torque Compensator.
* @return none.
* @example
* <CODE> MCAPP_TorqueCompInit(&torqueComp); </CODE>
*
*/
void MCAPP_TorqueCompInit(MCAPP_TORQUE_COMP_T *pTorqueComp)
{
    pTorqueComp->qOmegaAcc = 0;
    pTorqueComp->mechCount = 0;
}

/**
* <B> Function: MCAPP_TorqueCompensator(MCAPP_TORQUE_COMP_T * )  </B>
*
* @brief Function implementing Torque Compensator.
*
*
* @param Pointer to the data structure used by Torque Compensator.
* @return none.
* @example
* <CODE> MCAPP_TorqueCompensator(&torqueComp); </CODE>
*
*/
void MCAPP_TorqueComp(MCAPP_TORQUE_COMP_T *pTorqueComp)
{
    /* Calculates the block averaged speed of the motor that eliminates
     * torque ripple due to controller action */

    pTorqueComp->qOmegaAcc += (*pTorqueComp->pOmegaFilt);
    pTorqueComp->qOmegaMech = (*pTorqueComp->pOmegaFilt)/pTorqueComp->polePairs;

    pTorqueComp->qThetaMechStateVar += 
            __builtin_mulss(pTorqueComp->qOmegaMech,
                    pTorqueComp->normDeltaT);

    pTorqueComp->qThetaMech = (int16_t)(pTorqueComp->qThetaMechStateVar >> 15);

    pTorqueComp->mechCount++;
    if (pTorqueComp->qThetaMech > 0 && pTorqueComp->qThetaMechPrev < 0)
    {
      pTorqueComp->qOmegaAvg = __builtin_divsd(pTorqueComp->qOmegaAcc,
                                            pTorqueComp->mechCount);

      pTorqueComp->qOmegaAcc = 0;
      pTorqueComp->mechCount = 0;
    }
    pTorqueComp->qThetaMechPrev = pTorqueComp->qThetaMech;

}
