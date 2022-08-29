// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file id_ref.c
 *
 * @brief This module implements field weakening and MTPA algorithms to 
 * generate id current reference (FOC) required for extended speed operation of 
 * SPMSM/IPMSMs.
 *
 * Component: ID REFERENCE GENERATION
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

/* _Q15abs and _Q15sqrt function use */
#include <libq.h>

#include "id_ref.h"
#include "general.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">

static void MCAPP_FdWeakControl(MCAPP_FDWEAK_T *);
static void MCAPP_MTPAControl(MCAPP_MTPA_T *);

// </editor-fold>

/**
* <B> Function: MCAPP_FdWeakInit(MCAPP_FDWEAK_T * )  </B>
*
* @brief Function to reset variables used for Field Weakening Control.
*        .
*
* @param Pointer to the data structure containing Field Weakening Control
*        parameters.
* @return   none.
* @example
* <CODE> MCAPP_FdWeakInit(&fieldWeak); </CODE>
*
*/
void  MCAPP_IdRefGenerationInit(MCAPP_IDREFGEN_T *pIdRefGen)
{
    pIdRefGen->pFdWeak->IdRefFiltStateVar = 0;
    pIdRefGen->pmtpa->IdRefFiltStateVar = 0;
    pIdRefGen->pFdWeak->IdRef = 0;
    pIdRefGen->pmtpa->IdRef = 0;
}

/**
* <B> Function: MCAPP_IdReferenceGeneration(MCAPP_IDREFGEN_T * )  </B>
*
* @brief Function implementing Field Weakening Control
*
* @param Pointer to the data structure containing Field Weakening Control
*        parameters.
* @return   none.
* @example
* <CODE> MCAPP_IdReferenceGeneration(&idRefGen); </CODE>
*
*/
void MCAPP_IdRefGeneration(MCAPP_IDREFGEN_T *pIdRefGen)
{
    MCAPP_FDWEAK_T *pFdWeak = pIdRefGen->pFdWeak;
    MCAPP_MTPA_T *pmtpa = pIdRefGen->pmtpa;
    
    /* Calculate Flux Weakening Control current */
    
    MCAPP_FdWeakControl(pFdWeak);
        
    /* Calculate MTPA Current Reference if applicable */
    if(pmtpa->enable)
    {
        /* Calculate Mtpa Current */
        MCAPP_MTPAControl(pmtpa);
    }
    
    /* Ensure minimum MTPA Current */
    if(pFdWeak->IdRefFilt < pmtpa->IdRefFilt)
    {
        pIdRefGen->IdRefFilt = pFdWeak->IdRefFilt;
    }
    else
    {
        pIdRefGen->IdRefFilt = pmtpa->IdRefFilt;
    }
}

/**
* <B> Function: MCAPP_FdWeakControl(MCAPP_FDWEAK_T * )  </B>
*
* @brief Function implementing Field Weakening Control
*
* @param Pointer to the data structure containing Field Weakening Control
*        parameters.
* @return   none.
* @example
* <CODE> MCAPP_FdWeakControl(&fieldWeak); </CODE>
*
*/
uint16_t Override; 
int16_t IdrefOverride = -7100;
static void MCAPP_FdWeakControl(MCAPP_FDWEAK_T *pFdWeak)
{    
    const MCAPP_CONTROL_T *pCtrlParam = pFdWeak->pCtrlParam;
    const int16_t vdc = *(pFdWeak->pVdc);
    const MCAPP_MOTOR_T *pMotor = pFdWeak->pMotor;
    int16_t
        dIqs,           /* Delta_iqs = Current - previous iqs */
        LqsDIqs,        /* Inductance drop along Lqs */
        mds_sq,         /* Square of modulation index along D axis */
        mqs_ref,        /* Modulation index reference along Q axis */
        vqs_ref;        /* Q-axis voltage reference */

    /*
     * Steady state stator voltage equations:
     * Vds = Rs * Ids - Omega * Lqs * Iqs
     * Vqs = Rs * Iqs + Omega * Lds * Ids + Omega * Fdr
     */
    pFdWeak->OmegaLds =
            (int16_t)(__builtin_mulss(pCtrlParam->qVelRef, pMotor->qLdDt)>> 15);

    /* Omega * Lds calculation scaled by NORM_LSDTBASE_SCALE bits */
    pFdWeak->OmegaLds =
            (int16_t)(__builtin_mulss(pFdWeak->OmegaLds,
                            pCtrlParam->OmegaDtGain) >> 15);

    /* Rs*Iqs calculation scaled by Vbase / 2^15 */
    pFdWeak->RsIqs =
            (int16_t)(__builtin_mulss(pCtrlParam->qIqRef, pMotor->qRs) >>
                            (pMotor->RsScale));
    dIqs = pCtrlParam->qIqRef - pFdWeak->prevIqs;
    LqsDIqs =
            (int16_t)(__builtin_mulss(dIqs, pMotor->qLqDt) >>
            (pMotor->LsDtBaseScale));
    pFdWeak->LqsDIqs =
            (int16_t)((__builtin_mulss(pFdWeak->LqsDIqs,
                                        (32767-pFdWeak->LqsDIqsFiltConst)) +
                    __builtin_mulss(LqsDIqs,pFdWeak->LqsDIqsFiltConst)) >> 15);

         /* 
     * vqs_ref = sqrt(Vmax^2 - vds^2)
    */
    mds_sq = (int16_t)(__builtin_mulss(pFdWeak->pVdq->d, 
                                        pFdWeak->pVdq->d) >> 15);

    if(mds_sq >= VMAX_SQR_FDWEAK)
    {
        mds_sq = VMAX_SQR_FDWEAK;
    }

    mqs_ref = _Q15sqrt(VMAX_SQR_FDWEAK - mds_sq);

    /* vqs_ref scaled by Vbase/2^14 */
    vqs_ref = (int16_t)(__builtin_mulss(mqs_ref, vdc) >> 16);

    pFdWeak->OmegaLdsIds = 
            (vqs_ref - 
                    ((pFdWeak->RsIqs + pFdWeak->LqsDIqs) >> 1) 
                        - *(pFdWeak->pEstim->pEmag));

    /* Feedforward term ids_fw = 
     * (vqs_ref - rs*Iqs - Lqs*dIqs/dt - Emag)/(omega*Lds)
     */

    if(pFdWeak->OmegaLdsIds < 0)
    {    

        pFdWeak->IdRef = 
            (int16_t)(__builtin_divsd(
                        __builtin_mulss(pFdWeak->OmegaLdsIds, 
                                        (1 << (pMotor->LsDtBaseScale + 1))),
                                        pFdWeak->OmegaLds));
    }
    else
    {
        pFdWeak->IdRef = 0;
    }
    /* Clamp id_ref to 0 on positive side */
    if(pFdWeak->IdRef > 0)
    {
        pFdWeak->IdRef = 0;
    }
    else if(pFdWeak->IdRef < pFdWeak->IdRefMin)
    {
        /* Clamp id_ref to FD_WEAK_IDREF_LT on negative side  */
        pFdWeak->IdRef = pFdWeak->IdRefMin;
    }
    
    /*Filter for the FW Idref current*/
    pFdWeak->IdRefFiltStateVar +=
            __builtin_mulss((pFdWeak->IdRef - pFdWeak->IdRefFilt),
                                pFdWeak->IdRefFiltConst);
    pFdWeak->IdRefFilt =
                (int16_t)(pFdWeak->IdRefFiltStateVar >> 15);
    if(Override == 1)
    {
        pFdWeak->IdRefFilt = IdrefOverride;
    }
    
    pFdWeak->prevIqs = pCtrlParam->qIqRef;
}

/**
* <B> Function: MCAPP_MTPAControl(MCAPP_MTPA_T *)  </B>
*
* @brief Function implementing Maximum Torque per Ampere Control(MTPA)
*
* @param Pointer to the data structure containing MTPA Control parameters.
* @return none.
* @example
* <CODE> MCAPP_MTPAInit(&mtpaParam); </CODE>
*
*/
static void MCAPP_MTPAControl(MCAPP_MTPA_T *pmtpa)
{
    const MCAPP_MOTOR_T *pMotor = pmtpa->pMotor;
    int16_t sumOfSqrs, idRef, tmp;

    /* 4*L1*w => Left shift by 2 bits */
    pmtpa->OmegaL1Four =
                (int16_t)(__builtin_mulss(pmtpa->pCtrlParam->qVelRef,
            pMotor->qL1Dt)>> 13);

    /*Omega * L1 calculation scaled by NORM_LSDTBASE_SCALE bits */
    pmtpa->OmegaL1Four =
                (int16_t)(__builtin_mulss(pmtpa->OmegaL1Four,
                                pmtpa->pCtrlParam->OmegaDtGain) >> 15);

    /* Omega*Fdr calculation scaled by Vbase / 2^14 */
    tmp = *(pmtpa->pEstim->pEmag);

    pmtpa->OmegaFdrSq = (int16_t)(__builtin_mulss(tmp,tmp) >> 15);

    /* Divide by zero protection */
    if(pmtpa->OmegaL1Four == 0)
    {
        pmtpa->OmegaL1Four = 1;
    }

    /* Omega*L1*Iqs calculation scaled by Vbase/2^14 */
    pmtpa->OmegaL1Iqs =
        (int16_t)(__builtin_mulss(pmtpa->OmegaL1Four,
                                    pmtpa->pCtrlParam->qIqRef) >>
                               (pMotor->LsDtBaseScale + 1));

    pmtpa->OmegaL1IqsSq =
            (int16_t)(__builtin_mulss(pmtpa->OmegaL1Iqs,
                                pmtpa->OmegaL1Iqs) >> 15);

    sumOfSqrs = pmtpa->OmegaFdrSq + pmtpa->OmegaL1IqsSq;

    if(sumOfSqrs < 0)
    {
        sumOfSqrs = 0;
    }

    pmtpa->SqrtTerm = _Q15sqrt(sumOfSqrs);

    tmp = (-*(pmtpa->pEstim->pEmag) + pmtpa->SqrtTerm);

    /*
    * id_ref Scaled by Ibase / 2^15
    */
    idRef = (int16_t)(__builtin_divsd(
                (__builtin_mulss((1 << (pMotor->LsDtBaseScale + 1)),
                                    tmp)),
                (pmtpa->OmegaL1Four)));

    /* Clamp id_ref to MTPA_IDREF_CLAMP on positive side */
    if(idRef > (pmtpa->idRefMax) )
    {
        idRef = pmtpa->idRefMax;
    }
    else if(idRef < pmtpa->idRefMin)
    {
        /* Clamp id_ref to FD_WEAK_IDREF_LT on negative side  */
        idRef = pmtpa->idRefMin;
    }
    /* Calculate id_ref to Ibase / 2^15 scaling */
    pmtpa->IdRef = idRef;
    
    /*Filter for the FW Idref current*/
    pmtpa->IdRefFiltStateVar +=
            __builtin_mulss((pmtpa->IdRef - pmtpa->IdRefFilt),
                                pmtpa->IdRefFiltConst);
    
    pmtpa->IdRefFilt =
            (int16_t)(pmtpa->IdRefFiltStateVar >> 15);
}