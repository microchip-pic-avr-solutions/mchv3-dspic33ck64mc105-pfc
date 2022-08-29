// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file estim_atpll.h
 *
 * @brief This module implements ATPLL(Angle Tracking PLL ) Estimator.
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

#ifndef __ESTIM_ATPLL_H
#define __ESTIM_ATPLL_H

#ifdef __cplusplus
    extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include "motor_control.h"
#include "measure.h"
#include "foc_control_types.h"
#include "motor_params.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS">

typedef struct
{
    MC_ALPHABETA_T
        qEAlphaBeta,        /* Estimated Back EMF */
        qVIndAlphaBeta,     /* Inductance Drop */
        qVResAlphaBeta,     /* Resistance Drop */
        qLastVAlphaBeta,    /* Last Applied Voltage */
        qLastFAlphaBeta;    /* Last Stator Flux */

    MC_DQ_T
        qEdq,               /* Estimated Back EMF */
        qEdqFilt;           /* Filtered Back EMF */

    int16_t
        qCosTheta,          /* Cos(Theta) */
        qSinTheta,          /* Sin(Theta) */
        qE1,                /* EBeta * Cos(Theta) */
        qE2,                /* Ealpha * Sin(Theta) */
        qEerr,              /* Back EMF Error which is forced to zero*/
        qKpGain,            /* Kp Gain */
        qEmagExp,           /* Expected BEMF */
        qPIOut,             /* Unscaled PI Output */
        qKiIn,              /* I controller input */
        qOmega,             /* Estimated Speed */
        qOmegaFilt,         /* Filtered Estimated Speed */
        qOmegaPIOut,        /* PI Output */
        qOmegaPIOutFilt,    /* PI Output Filtered */
        qTheta,             /* Estimated Rotor Angle */
        qThetaFlux,         /* Estimated Rotor Flux Angle */
        qLaa,               /* Lalpha-alpha */
        qLab,               /* Lalpha-beta */
        qLba,               /* Lbeta-alpha */
        qLbb,               /* Lbeta-beta */
        qSin2Theta,         /* Sin(2*Theta) */
        qCos2Theta,         /* Cos(2*Theta) */
        qL1Cos2Theta,       /* L1 * Cos(2*Theta) */
        qL1Sin2Theta,       /* L1 * Sin(2*Theta) */
        qOmegaL1,           /* Omega L1 */
        qEmag,              /* Magnitude of BEMF */
        qEmagFilt,          /* Filtered BEMF */
        SAL_OBS,            /* Saliency in Observer to be included */
        qEmagFiltConst,     /* BEMF Filter Constant */
        qOmegaFiltConst,    /* Omega Filter Constant */
        qOmegaPIFiltConst;  /* PI Output Filter Constant */

    int32_t
        qKiOut,             /* I controller output */
        qKpOut,             /* P controller output */
        qEaCosTheta,        /* Ealpha * cos(Theta) */
        qEaSinTheta,        /* Ealpha * sin(Theta) */
        qEbCosTheta,        /* Ebeta *  cos(Theta) */
        qEbSinTheta,        /* Ebeta *  sin(Theta) */
        qOmegaPIOutFiltTemp,/* PI Controller output filter state variable */
        qOmegaFiltStateVar, /* Speed in high precision */
        qThetaStateVar,     /* Angle in high precision */
        qEdFiltStateVar,    /* Ed filter variable */
        qEqFiltStateVar,    /* Eq filter variable */
        qEmagFiltStateVar;  /* Emag filter variable */

    const MCAPP_CONTROL_T *pCtrlParam;
    const int16_t *pVdc;
    const MC_ALPHABETA_T *pIAlphaBeta;
    const MC_ALPHABETA_T *pVAlphaBeta;
    const MCAPP_MOTOR_T *pMotor;

} MCAPP_ESTIMATOR_ATPLL_T;

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void MCAPP_EstimatorATPLLInit (MCAPP_ESTIMATOR_ATPLL_T *);
void MCAPP_EstimatorATPLL (MCAPP_ESTIMATOR_ATPLL_T *);

// </editor-fold>

#ifdef __cplusplus
    }
#endif

#endif /* end of __ESTIM_ATPLL_H */
