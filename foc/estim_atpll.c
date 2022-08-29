// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file estim_atpll.c
 *
 * @brief This module implements ATPLL(Angle Tracking PLL ) Estimator.
 * This is a sensor-less speed observer based on motor back EMF.
 *
 * Component: ESTIMATOR
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

#include "estim_atpll.h"

/* For PI_BY_2 */
#include "general.h"

// </editor-fold>

/**
* <B> Function: void MCAPP_EstimatorATPLLInit(MCAPP_ATPLL_ESTIMATOR_T *)  </B>
*
* @brief Function to reset ATPLL Estimator Data Structure variables.
*
* @param    pointer to the data structure containing ATPLL Estimator parameters.
* @return   none.
* @example
* <CODE> MMCAPP_EstimatorATPLLInit(&estimator); </CODE>
*
*/
void MCAPP_EstimatorATPLLInit(MCAPP_ESTIMATOR_ATPLL_T *pEstim)
{
    pEstim->qOmegaFiltStateVar = 0;
    pEstim->qThetaStateVar = 0;
    pEstim->qKiOut = 0;
    pEstim->qOmegaPIOutFiltTemp = 0;
    pEstim->qEdFiltStateVar = 0;
    pEstim->qEqFiltStateVar = 0;
    pEstim->qTheta = 0;
    pEstim->qOmegaFilt = 0;
}

/**
* <B> Function: void MCAPP_EstimatorATPLL(MCAPP_ATPLL_ESTIMATOR_T *)  </B>
*
* @brief Observer to determine rotor speed and position based on
* motor parameters and feedbacks.
*
* @param    pointer to the data structure containing ATPLL Estimator parameters.
* @return   none.
* @example
* <CODE> MCAPP_EstimatorATPLL(&estimator); </CODE>
*
*/
void MCAPP_EstimatorATPLL(MCAPP_ESTIMATOR_ATPLL_T *pEstim)
{
    const MCAPP_MOTOR_T *pMotor = pEstim->pMotor;
    const MC_ALPHABETA_T *pIAlphaBeta = pEstim->pIAlphaBeta;
    const MCAPP_CONTROL_T *pCtrlParam = pEstim->pCtrlParam;
    const int16_t vdc = *(pEstim->pVdc);
    int16_t
            filtErr,        /* Filter error */
            kiInFactor,     /* KI for estimator */
            eAlSqr,         /* E_alpha square */
            eBeSqr,         /* E_beta square */
            PIOutput,       /* PI output */
            angle;          /* BEMF angle */

    MC_ALPHABETA_T  fluxAlphaBeta;     /* Flux in alpha-beta */
    MC_SINCOS_T     estimSinCos;        /* Sine-cosine for estimator */  

    if(pEstim->SAL_OBS)
    {
        /* If motor has significant saliency ,execute :*/

        angle = pEstim->qTheta << 1;
        MC_CalculateSineCosine_Assembly_Ram(angle, &estimSinCos);
        pEstim->qCos2Theta = estimSinCos.cos;
        pEstim->qSin2Theta = estimSinCos.sin;
        pEstim->qL1Cos2Theta = (int16_t)(__builtin_mulss(pMotor->qL1Dt,
                                                    pEstim->qCos2Theta) >> 15);
        pEstim->qL1Sin2Theta = (int16_t)(__builtin_mulss(pMotor->qL1Dt,
                                                    pEstim->qSin2Theta) >> 15);

        pEstim->qLaa = pMotor->qL0Dt - pEstim->qL1Cos2Theta;
        pEstim->qLab = -pEstim->qL1Sin2Theta;
        pEstim->qLba = -pEstim->qL1Sin2Theta;
        pEstim->qLbb = pMotor->qL0Dt + pEstim->qL1Cos2Theta;
    }

    /*
       Stator voltage equations:

       Valpha = Rs * Ialpha + d((Ls/dT) * (Ialpha)) + BEMFalpha
       Vbeta  = Rs * Ibeta + d((Ls/dT) * (Ibeta)) + BEMFbeta

    */

    /* To avoid overflow of calculated BEMFs, the stator equations are scaled
       with 2 */

    pEstim->qLastVAlphaBeta.alpha =
            (int16_t)(__builtin_mulss(pEstim->pVAlphaBeta->alpha,
                                                vdc) >> (15 + 1));
    pEstim->qLastVAlphaBeta.beta =
            (int16_t)(__builtin_mulss(pEstim->pVAlphaBeta->beta,
                                                vdc) >> (15 + 1));

    if(pEstim->SAL_OBS) /* There is significant saliency in the motor */
    {
        /* If motor has significant saliency ,execute : */
        fluxAlphaBeta.alpha =
            (int16_t)(((__builtin_mulss(pEstim->qLaa, pIAlphaBeta->alpha)) +
                           (__builtin_mulss(pEstim->qLab, pIAlphaBeta->beta)))
                                >> (pMotor->LsDtBaseScale + 1));

        fluxAlphaBeta.beta =
            (int16_t)(((__builtin_mulss(pEstim->qLba, pIAlphaBeta->alpha)) +
                           (__builtin_mulss(pEstim->qLbb, pIAlphaBeta->beta)))
                                >> (pMotor->LsDtBaseScale + 1));
    }
    else
    {
        /* if motor has no significant saliency , execute : */
        fluxAlphaBeta.alpha =
            (int16_t)(__builtin_mulss(pMotor->qL0Dt, pIAlphaBeta->alpha)
                                >> (pMotor->LsDtBaseScale + 1));

        fluxAlphaBeta.beta =
            (int16_t)(__builtin_mulss(pMotor->qL0Dt, pIAlphaBeta->beta)
                                >> (pMotor->LsDtBaseScale + 1));
    }
    pEstim->qVIndAlphaBeta.alpha = fluxAlphaBeta.alpha -
                                            pEstim->qLastFAlphaBeta.alpha;

    pEstim->qVIndAlphaBeta.beta = fluxAlphaBeta.beta -
                                            pEstim->qLastFAlphaBeta.beta;

    pEstim->qVResAlphaBeta.alpha =
            (int16_t)(__builtin_mulss( pMotor->qRs, pIAlphaBeta->alpha) >>
                            (pMotor->RsScale + 1));

    pEstim->qVResAlphaBeta.beta =
            (int16_t)(__builtin_mulss( pMotor->qRs, pIAlphaBeta->beta) >>
                            (pMotor->RsScale + 1));

    pEstim->qEAlphaBeta.alpha = pEstim->qLastVAlphaBeta.alpha -
                                pEstim->qVIndAlphaBeta.alpha -
                                pEstim->qVResAlphaBeta.alpha;

    pEstim->qEAlphaBeta.beta = pEstim->qLastVAlphaBeta.beta -
                                pEstim->qVIndAlphaBeta.beta -
                                pEstim->qVResAlphaBeta.beta;

    /* Update  LastIalpha and LastIbeta */

    pEstim->qLastFAlphaBeta.alpha = fluxAlphaBeta.alpha;
    pEstim->qLastFAlphaBeta.beta	= fluxAlphaBeta.beta;

    /* Estimate BEMFs. Calculate BEMF error */

    MC_CalculateSineCosine_Assembly_Ram(pEstim->qTheta, &estimSinCos);
    pEstim->qCosTheta = estimSinCos.cos;
    pEstim->qSinTheta = estimSinCos.sin;

    pEstim->qE1 = (int16_t)(__builtin_mulss(pEstim->qEAlphaBeta.beta,
                                pEstim->qCosTheta) >> 15);

    pEstim->qE2 = (int16_t)(__builtin_mulss(pEstim->qEAlphaBeta.alpha,
                                pEstim->qSinTheta) >> 15);
    
    pEstim->qEerr = (pEstim->qEerr + (pEstim->qE1 - pEstim->qE2 )) >> 1;
    
    /* Filter BEMF magnitude */    
    eAlSqr = (int16_t)(__builtin_mulss(pEstim->qEAlphaBeta.alpha,
                    pEstim->qEAlphaBeta.alpha) >> 15);
    eBeSqr = (int16_t)(__builtin_mulss(pEstim->qEAlphaBeta.beta,
                    pEstim->qEAlphaBeta.beta) >> 15);

    pEstim->qEmag = (_Q15sqrt(eAlSqr + eBeSqr));

    filtErr = pEstim->qEmag - pEstim->qEmagFilt;
    pEstim->qEmagFiltStateVar += 
                            __builtin_mulss(filtErr, pEstim->qEmagFiltConst);
    pEstim->qEmagFilt = (int16_t)(pEstim->qEmagFiltStateVar >> 15);

    /*  Calculate deltaOmega needed to align estimator with BEMF */

    /* PI Output Limit */
    pEstim->qEmagExp = (__builtin_divsd(
                                    __builtin_mulss(pCtrlParam->qVelRef,
                                            (1<<(pMotor->invKFIBaseScale - 1))),
                                    pMotor->qInvKFIBase));
    /* Kp Output */
    pEstim->qKpOut = (__builtin_mulss(pEstim->qEerr, pEstim->qKpGain));

    kiInFactor = (int16_t)(__builtin_mulss(pCtrlParam->qVelRef, 3276)>> 15);

    pEstim->qKiIn = (int16_t)(__builtin_mulss(pEstim->qEerr ,kiInFactor) >> 15);

    pEstim->qKiOut += (__builtin_mulss(pEstim->qKiIn, pCtrlParam->normDeltaT));

    /* Compensating effect of BEMF constant */
    pEstim->qPIOut = (int16_t)((pEstim->qKpOut + pEstim->qKiOut) >> 15);
    
    pEstim->qOmegaPIOut =
                (int16_t)(__builtin_mulss(pEstim->qPIOut, pMotor->qInvKFIBase)>>
									(pMotor->invKFIBaseScale-1));

    /* Filter on PI Output */
    filtErr = pEstim->qOmegaPIOut - pEstim->qOmegaPIOutFilt;
    pEstim->qOmegaPIOutFiltTemp +=
                            __builtin_mulss(filtErr, pEstim->qOmegaPIFiltConst);
    pEstim->qOmegaPIOutFilt = (int16_t)(pEstim->qOmegaPIOutFiltTemp >> 15);

    /* Calculate Speed and angle */
    pEstim->qOmega = (int16_t)(pEstim->qOmegaPIOutFilt + pCtrlParam->qVelRef);
    pEstim->qThetaStateVar += 
                        __builtin_mulss(pEstim->qOmega, pCtrlParam->normDeltaT);

    pEstim->qTheta = (int16_t)( pEstim->qThetaStateVar >> 15);
    
    pEstim->qThetaFlux = pEstim->qTheta - PI_BY_2;

    /* Filter on estimated speed for outer speed loop */
    filtErr = pEstim->qOmega - pEstim->qOmegaFilt;

    pEstim->qOmegaFiltStateVar += 
                            (__builtin_mulss(pEstim->qOmegaFiltConst, filtErr));

    pEstim->qOmegaFilt =
            (int16_t)(pEstim->qOmegaFiltStateVar >> 15);
}
