// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file id_ref.h
 *
 * @brief This module implements Id current reference generation of PMSM/IPMSM.
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

#ifndef __ID_REF_H
#define __ID_REF_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>

#include "foc_control_types.h"
#include "measure.h"
#include "estim_interface.h"
#include "motor_control.h"
#include "motor_params.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    int16_t
        OmegaLds,           /* D-axis Inductive Reactance */
        OmegaLqs,           /* Q-axis Inductive Reactance */
        OmegaLdsIds,        /* Stator voltage drop along Lds */
        LqsDIqs,            /* Voltage drop across Lqs */
        LqsDIqsFiltConst,   /* Filter constant for Q-axis Inductive Drop */
        prevIqs,            /* Previous Iqs Reference */
        IdRef,              /* Id Current reference */
        IdRefFilt,          /* Filtered Id Current reference */
        IdRefFiltConst,     /* Filter constant for Id */
        IdRefMin,           /* Lower limit on IdRef */
        RsIds,              /* Stator voltage drop along Rds */
        RsIqs;              /* Stator voltage drop along Rqs */
            
    int32_t
        IdRefFiltStateVar;  /* Accumulation variable for IdRef filter */

    const MCAPP_CONTROL_T *pCtrlParam;
    const int16_t *pVdc;
    const MCAPP_ESTIMATOR_T *pEstim;
    const MC_DQ_T *pVdq;
    const MCAPP_MOTOR_T *pMotor;

} MCAPP_FDWEAK_T;

typedef struct
{
    int16_t
        OmegaL1Four,        /* 4wL1 */
        OmegaL1Iqs,         /* Voltage drop due to saliency */
        OmegaFdrSq,         /* BEMF Squared */
        OmegaL1IqsSq,       /* (4wL1) Squared */
        SqrtTerm,           /* Square root term */
        IdRef,              /* IdRef due to MTPA */
        IdRefFilt,          /* Filtered Id Current reference */
        enable,             /* MTPA Enable flag */
        idRefMax,           /* Upper limit on IdRef due to MTPA */
        idRefMin,           /* Lower limit on IdRef due to MTPA */
        IdRefFiltConst;     /* Filter constant for Id */

    int32_t
        IdRefFiltStateVar;  /* Accumulation variable for IdRef filter */
    
    const MCAPP_CONTROL_T *pCtrlParam;
    const MCAPP_ESTIMATOR_T *pEstim;
    const MCAPP_MOTOR_T *pMotor;

} MCAPP_MTPA_T;

typedef struct
{
    int16_t 
        IdRefFilt;          /* Filtered Id Current reference */
    
    MCAPP_FDWEAK_T
        fdWeak;             /* Flux Weakening Structure */
    
    MCAPP_MTPA_T
        mtpa;               /* MTPA Structure */
    
    MCAPP_FDWEAK_T 
        *pFdWeak;           /* Pointer for Flux Weakening */
    
    MCAPP_MTPA_T 
        *pmtpa;             /* Pointer for MTPA */

} MCAPP_IDREFGEN_T;


// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
void MCAPP_IdRefGenerationInit(MCAPP_IDREFGEN_T *);
void MCAPP_IdRefGeneration(MCAPP_IDREFGEN_T *);

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* end of __ID_REF_H */
