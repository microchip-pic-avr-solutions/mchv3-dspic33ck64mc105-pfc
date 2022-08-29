// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
* @file foc_types.h
*
* @brief This module has variable type definitions of data structure
* holding foc parameters and enumerated constants.
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

#ifndef __FOC_TYPES_H
#define __FOC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include "pi.h"
#include "foc_control_types.h"
#include "estim_interface.h"
#include "estim_atpll.h"
#include "id_ref.h"
#include "motor_control.h"
#include "stall_detect.h"
#include "motor_params.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="ENUMERATED CONSTANTS ">

typedef enum
{
    FOC_INIT = 0,              /* Initialize control parameters */
    FOC_RTR_LOCK = 1,          /* Rotor Lock */
    FOC_OPEN_LOOP = 2,         /* Open Loop */
    FOC_CLOSE_LOOP = 3,        /* Closed Loop */
    FOC_FAULT = 4,             /* Motor is in Fault */

}FOC_CONTROL_STATE_T;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    int16_t
        *pIa,               /* Pointer for Ia */
        *pIb,               /* Pointer for Ib */
        *pVdc,              /* Pointer for Vdc */
        faultStatus,        /* Fault Status */
        focState;           /* FOC State */       
    
    MCAPP_PI_T
        piIq,               /* Parameters for PI Q axis controllers */
        piId,               /* Parameters for PI D axis controllers */
        piSpeed;            /* Parameters for PI Speed controllers */
    
    MC_SINCOS_T
        sincosTheta;        /* Sincos parameters */
    
    MC_DQ_T
        vdq,                /* Vdq */
        idq;                /* Idq */
    
    MC_ABC_T
        iabc,               /* Iabc */
        vabc;               /* Vabc */
    
    MC_ALPHABETA_T
        ialphabeta,         /* IalphaBeta */
        valphabeta;         /* Duty_alphaBeta */
    
    MC_DUTYCYCLEOUT_T
        *pPWMDuty;            /* Duties out */
    
    MCAPP_ESTIMATOR_ATPLL_T
        estimATPLL;         /* Estimator Structure */

    MCAPP_ESTIMATOR_T
        estimInterface;     /* Estimator Interface Structure */

    MCAPP_IDREFGEN_T
        idRefGen;
    
    MCAPP_CONTROL_T
        ctrlParam;          /* Parameters for control references */

    MCAPP_MOTOR_T
        *pMotor;            /* Pointer for Motor Parameters */
    
    MCAPP_STALL_DETECT_T
        stallDetect;        /* Stall Detection Structure */
    
    uint16_t pwmPeriod;     /* PWM Period */

}MCAPP_FOC_T;

typedef enum
{
    STALL_DETECTED = 1,
}MCAPP_FAULT_STATUS_T;

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* end of __FOC_TYPES_H */
