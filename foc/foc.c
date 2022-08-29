// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file foc.c
 *
 * @brief This module implements Field Oriented Control(FOC).
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include <libq.h>

#include "foc.h"
#include "general.h"
#include "hal/port_config.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">

static void MCAPP_FOCFeedbackPath(MCAPP_FOC_T *);
static void MCAPP_FOCForwardPath(MCAPP_FOC_T *);

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="GLOBAL FUNCTIONS ">

void MCAPP_FOCStateMachine(MCAPP_FOC_T *);
void MCAPP_FOCInit(MCAPP_FOC_T *);

// </editor-fold>

/**
* <B> Function: void MCAPP_FOCInit(MCAPP_FOC_T *)  </B>
*
* @brief Executes FOC Parameters Initialization.
*
* @param Pointer to the data structure containing FOC parameters.
* @return none.
* @example
* <CODE> MCAPP_FOCInit(&mc); </CODE>
*
*/

void MCAPP_FOCInit(MCAPP_FOC_T *pFOC)
{
    MCAPP_CONTROL_T *pCtrlParam = &pFOC->ctrlParam;
    
    MCAPP_PIInit(&pFOC->piId);
    MCAPP_PIInit(&pFOC->piIq);
    MCAPP_PIInit(&pFOC->piSpeed);
    MCAPP_EstimatorATPLLInit(&pFOC->estimATPLL);
    MCAPP_IdRefGenerationInit(&pFOC->idRefGen);
    MCAPP_StallDetectInit(&pFOC->stallDetect);
    
    pCtrlParam->lockTime = 0;
    pCtrlParam->speedRampSkipCnt = 0;
    pCtrlParam->OLTheta = 0;
    pCtrlParam->qVelRef = 0;
    pCtrlParam->OLThetaSum = 0;
    pCtrlParam->qIdRef = 0;
    pCtrlParam->qIqRef = 0;
    
    pFOC->faultStatus = 0;
    
    pFOC->pPWMDuty->dutycycle3 = 0;
    pFOC->pPWMDuty->dutycycle2 = 0;
    pFOC->pPWMDuty->dutycycle1 = 0;

}

/**
* <B> Function: void MCAPP_FOCControlLoop(MCAPP_FOC_T *)  </B>
*
* @brief Executes Speed and Current Control Loops and performs actions
*        associated with control state transition required for FOC.
*
* @param Pointer to the data structure containing FOC parameters.
* @return none.
* @example
* <CODE> MCAPP_FOCControlLoop(&mc); </CODE>
*
*/
void MCAPP_FOCStateMachine(MCAPP_FOC_T *pFOC)
{
    int16_t qOmega;

    MCAPP_CONTROL_T *pCtrlParam = &pFOC->ctrlParam;

    qOmega = *(pFOC->estimInterface.pOmega);
    
    switch (pFOC->focState)
    {
        case FOC_INIT:
            MCAPP_FOCInit(pFOC);            
            break;
            
        case FOC_RTR_LOCK:
            MCAPP_FOCFeedbackPath(pFOC);
            pFOC->estimInterface.qTheta = pCtrlParam->OLTheta;
            
            pCtrlParam->qIqRef = pCtrlParam->lockCurrent;
            
            MCAPP_FOCForwardPath(pFOC);
            
            if (pCtrlParam->lockTime < pCtrlParam->lockTimeLimit)
            {
                pCtrlParam->lockTime++;
            }
            else
            {
                pCtrlParam->lockTime = 0;
                pCtrlParam->speedRampSkipCnt = 0;
                
                pCtrlParam->OLThetaSum = 0;
                pCtrlParam->OLTheta = (int16_t)(pCtrlParam->OLThetaSum >> 15);            
                pFOC->estimInterface.qTheta  = pCtrlParam->OLTheta;
                pFOC->focState = FOC_OPEN_LOOP;
            }
            
        break;
        
        case FOC_OPEN_LOOP:
            MCAPP_FOCFeedbackPath(pFOC); 
            
            if(pCtrlParam->OLCurrent < pCtrlParam->OLCurrentMax)
            {
                if(pCtrlParam->OLCurrentRampSkipCnt >= 
                        pCtrlParam->OLCurrentRampSkipCntLimit)
                {
                    pCtrlParam->OLCurrentRampSkipCnt = 0;
                    pCtrlParam->OLCurrent += pCtrlParam->OLCurrentRampRate;
                }
                else
                {
                    pCtrlParam->OLCurrentRampSkipCnt++;
                }
                
            }
            
            if(pCtrlParam->qVelRef < pFOC->pMotor->qMaxOLSpeed)
            {
                if(pCtrlParam->speedRampSkipCnt >= 
                                pCtrlParam->speedRampSkipCntLimit)
                {
                    pCtrlParam->speedRampSkipCnt = 0;
                    pCtrlParam->qVelRef += pCtrlParam->OLSpeedRampRate;
                }
                else
                {
                    pCtrlParam->speedRampSkipCnt++;
                }
            }
            else if(pCtrlParam->openLoop == 0)
            {
                pFOC->estimInterface.qThetaOffset = pCtrlParam->OLTheta -
                                                   pFOC->estimATPLL.qThetaFlux;
                /* Set current reference to existing Iq */
                pFOC->piSpeed.qdSum = ((int32_t)pCtrlParam->qIqRef) << 15;
                
                /* Set EdFilt variable to existing Emag */
                pFOC->stallDetect.qEdFiltStateVar = 
                        ((int32_t)(*pFOC->estimInterface.pEmag)) << 15;

               /* Reset speed ramp count */
                pCtrlParam->speedRampSkipCnt = 0;          

                pFOC->focState = FOC_CLOSE_LOOP;

            }
            pCtrlParam->OLThetaSum += __builtin_mulss(pCtrlParam->qVelRef, 
                    pCtrlParam->normDeltaT);
            
            pCtrlParam->OLTheta = (int16_t)(pCtrlParam->OLThetaSum >> 15);
            
            pFOC->estimInterface.qTheta  = pCtrlParam->OLTheta;

            pCtrlParam->qIqRef = pCtrlParam->OLCurrent;
            
            MCAPP_FOCForwardPath(pFOC);
            break;
            
        case FOC_CLOSE_LOOP:

            /* Close the loop slowly */            
            if(pFOC->estimInterface.qThetaOffset > 0)
            {
                pFOC->estimInterface.qThetaOffset--;
            }
            else if(pFOC->estimInterface.qThetaOffset < 0)
            {
                pFOC->estimInterface.qThetaOffset++;
            }
            else
            {
                pFOC->estimInterface.qThetaOffset = 0;
            }

            pFOC->estimInterface.qTheta  = pFOC->estimATPLL.qThetaFlux +
                               pFOC->estimInterface.qThetaOffset;

            MCAPP_FOCFeedbackPath(pFOC);
            
            /* Closed Loop Speed Ramp */
            pCtrlParam->qDiff = pCtrlParam->qVelRef - 
                                (int16_t)pCtrlParam->qTargetVelocity;

            if(pCtrlParam->qDiff < 0)
            {
                if(pCtrlParam->speedRampSkipCnt >= 
                                pCtrlParam->speedRampIncLimit)
                {
                    pCtrlParam->speedRampSkipCnt = 0;
                    pCtrlParam->qVelRef = pCtrlParam->qVelRef + 
                                                    pCtrlParam->CLSpeedRampRate;
                }
            }
            else if(pCtrlParam->qDiff > 0)
            {
                if(pCtrlParam->speedRampSkipCnt >= 
                                pCtrlParam->speedRampDecLimit)
                {
                    pCtrlParam->speedRampSkipCnt = 0;
                    pCtrlParam->qVelRef = pCtrlParam->qVelRef - 
                                    pCtrlParam->CLSpeedRampRate;
                }
            }
            else
            {
                pCtrlParam->speedRampSkipCnt = 0;
            }
            pCtrlParam->speedRampSkipCnt++;

            /* Execute Outer Speed Loop - Generate Iq Reference Generation */

            pFOC->piSpeed.qInMeas = qOmega;
            pFOC->piSpeed.qInRef  = pCtrlParam->qVelRef;
            MCAPP_PIController(&pFOC->piSpeed);

            pCtrlParam->qIqRef = pFOC->piSpeed.qOut;

            /* Generate Id Reference : Flux Weakening and MTPA Control */
            MCAPP_IdRefGeneration(&pFOC->idRefGen);
            pCtrlParam->qIdRef = pFOC->idRefGen.IdRefFilt;
            
//            if(pFOC->stallDetect.enable == 1)
//            {
//                if(MCAPP_StallDetect(&pFOC->stallDetect))
//                {
//                    pFOC->faultStatus = STALL_DETECTED;
//                    pFOC->focState = FOC_FAULT;
//                }                
//            }            
            
            MCAPP_FOCForwardPath(pFOC);
            break;

        case FOC_FAULT:
            
            break;
        
        default:
            pFOC->focState = FOC_FAULT;
            break;

    } /* End Of switch - case */
}

/**
* <B> Function: void MCAPP_FOCFeedbackPath (MCAPP_FOC_T *)  </B>
*
* @brief Function executing Field Oriented Control(FOC).
*
* @param Pointer to the data structure containing FOC parameters.
* @return none.
* @example
* <CODE> MCAPP_FOCFeedbackPath(&mc); </CODE>
*
*/
static void MCAPP_FOCFeedbackPath(MCAPP_FOC_T *pFOC)
{
    pFOC->iabc.a = *(pFOC->pIa);
    pFOC->iabc.b = *(pFOC->pIb);
    pFOC->iabc.c = -pFOC->iabc.a - pFOC->iabc.b;
    
    /* Perform Clark & Park transforms to generate d axis and q axis currents */
    MC_TransformClarke_Assembly(&pFOC->iabc, &pFOC->ialphabeta);
    MC_TransformPark_Assembly(&pFOC->ialphabeta, 
                                &pFOC->sincosTheta, &pFOC->idq);

    /* Estimate Speed and rotor position */
    MCAPP_EstimatorATPLL(&pFOC->estimATPLL);
    
}

/**
* <B> Function: void MCAPP_FOCForwardPath(MCAPP_FOC_T *)  </B>
*
* @brief Executes Current Control Loops required for FOC.
*
* @param Pointer to the data structure containing FOC parameters.
* @return none.
* @example
* <CODE> MCAPP_FOCForwardPath(&mc); </CODE>
*
*/

static void MCAPP_FOCForwardPath(MCAPP_FOC_T *pFOC)
{
    int16_t tmp;
    
    /** Execute inner current control loops */
    /* Execute PI Control of Q axis. */
    pFOC->piId.qInMeas = pFOC->idq.d;
    pFOC->piId.qInRef  = pFOC->ctrlParam.qIdRef;
    MCAPP_PIController(&pFOC->piId);
    pFOC->vdq.d = pFOC->piId.qOut;

    /* Generate Q axis current reference based on available voltage and D axis
       voltage */
    tmp = (int16_t)(__builtin_mulss(pFOC->piId.qOut,pFOC->piId.qOut)>>15);

    tmp = VMAX_SQR - tmp;
    pFOC->piIq.qOutMax = _Q15sqrt(tmp);
    pFOC->piIq.qOutMin = -(pFOC->piIq.qOutMax); 
    
    /* Execute PI Control of Q axis. */
    pFOC->piIq.qInMeas = pFOC->idq.q;
    pFOC->piIq.qInRef  = pFOC->ctrlParam.qIqRef;
    MCAPP_PIController(&pFOC->piIq);
    pFOC->vdq.q = pFOC->piIq.qOut;
    
    /* Calculate sin and cos of theta (angle) */
    MC_CalculateSineCosine_Assembly_Ram(pFOC->estimInterface.qTheta, 
                                            &pFOC->sincosTheta);

    /* Perform inverse Clarke and Park transforms and generate phase voltages.*/
    MC_TransformParkInverse_Assembly(&pFOC->vdq, &pFOC->sincosTheta, 
                                                        &pFOC->valphabeta);

    /* Calculate Vr1,Vr2,Vr3 from qValpha, qVbeta */
    MC_TransformClarkeInverseSwappedInput_Assembly(&pFOC->valphabeta, 
                                                        &pFOC->vabc);

    /* Execute space vector modulation and generate PWM duty cycles */
    MC_CalculateSpaceVectorPhaseShifted_Assembly(&pFOC->vabc, pFOC->pwmPeriod,
                                                    pFOC->pPWMDuty);
}

