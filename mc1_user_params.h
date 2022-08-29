// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_user_params.h
 *
 * @brief This file has definitions to be configured by the user for spinning
 * 		  motor 1 using field oriented control.
 *
 * Component: APPLICATION (motor control 1 - mc1)
 * Motor : Godrej Airconditioner Compressor Motor
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

#ifndef __MC1_USER_PARAMS_H
#define __MC1_USER_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>

#include "general.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="DEFINITIONS/MACROS ">

/* Open loop continuous functioning ;Closed loop transition disabled.  */
#undef OPEN_LOOP_FUNCTIONING

/** Motor Parameters */
/** The following values are given in the .xlsx file. */             

/* Speed range of the motor in RPM - given by the Motor manufacturer */
#define     MINIMUM_SPEED_RPM   1450
#define     MAXIMUM_SPEED_RPM   7200

/* open loop speed ramp up end value */
#ifdef      OPEN_LOOP_FUNCTIONING
#define     END_SPEED_RPM       500
#else
#define     END_SPEED_RPM       300
#endif

/* Number of pole pairs of motor. */
#define     POLEPAIRS           3 //5

/* Normalized Rs value */
#define     NORM_RS             3076 //2796
#define     NORM_RS_SCALE       0   /* 2^NORM_RS_SCALE is the scaling */

/* normalized L0/dt value */
/* L0 = (Lq + Ld)/2 */
#define     NORM_L0DTBASE       9466 //10160

/* normalized L1/dt value */
/* L1 = (Ld - Lq)/2 */
#define     NORM_L1DTBASE       -1944 //0

/* All inductance scaled to 1 base */
#define     NORM_LSDTBASE_SCALE 5 //2    /* 2^NORM_LSDTBASE_SCALE is the scaling */

/* normalized inv kfi at base speed */
#define     NORM_INVKFIBASE     	13361 //17651
#define		NORM_INVKFIBASE_SCALE	1   //2 /* 2^NORM_INVKFIBASE_SCALE is
                                            //the scaling */

#define     NORM_LDLQ_THRESHOLD     (0.25) /* Saliency of atleast 25% must */
    
/* rated current: peak current drawn at rated load */
#define     RATED_CURRENT           (float)(5.0) //(3.0)

/**************  support xls file definitions end **************/
    
/* open loop startup constants */
/* the following values depends on the PWM frequency, */
/* lock time is the time needed for motor's poles alignment
 before the open loop speed ramp up */
#define     LOCK_TIME           30000 //4000
/* Locking Current */
#define     LOCK_CURRENT  		(float)(3.5)  //1.41
    
/* Open Loop Speed Reference Ramp */
#define     OPENLOOP_SPD_RATE               1
#define     OPENLOOP_SPD_COUNT_LIMIT        10
/* open loop q current setup - */
#define     MIN_OPENLOOP_CURRENT         	(float)(3.5) //1.41
#define     MAX_OPENLOOP_CURRENT         	(float)(3.5) //1.41
#define     OPENLOOP_CURRENT_RATE           1
#define     OPENLOOP_CURRENT_COUNT_LIMIT    7
    
/* Closed Loop Speed Reference Ramp */
#define     SPD_RAMP_UP_COUNT_LIMIT     45
#define     SPD_RAMP_DN_COUNT_LIMIT     30
#define     SPEEDREFRAMP                1

/******** D Control Loop Coefficients *******/
#define     D_CURRCNTR_PTERM    Q15(0.305)
#define     D_CURRCNTR_ITERM    1
#define     D_CURRCNTR_OUTMAX   Q15(VMAX_FLOAT)
#define     D_CURRCNTR_KPSCALE  2

/******** Q Control Loop Coefficients *******/
#define     Q_CURRCNTR_PTERM    Q15(0.305)
#define     Q_CURRCNTR_ITERM    1
#define     Q_CURRCNTR_OUTMAX   Q15(VMAX_FLOAT)
#define     Q_CURRCNTR_KPSCALE  2

/*** Velocity Control Loop Coefficients *****/
#define     SPEEDCNTR_PTERM     Q15(0.8)
#define     SPEEDCNTR_ITERM     10
#define     SPEEDCNTR_OUTMAX    (float)(15.0)
#define     SPEEDCNTR_KPSCALE   0
  
/** Estimator-ATPLL Parameters */
#define     OMEGA_PI_FILT_CONST 30000
#define     OMEGA_FILT_CONST    3200
#define     ESTIMATOR_KP_GAIN   Q15(0.95)
#define     EMAG_FILT_CONST     500
#define     LQSIQS_FILT_CONST   3600

/** MTPA Control Parameters */ 
#define     MTPA_IDREF_LT       (float)(-10.5)   //-3
#define     MTPA_IDREF_FILT_CONST    500

/** Field Weakening Control Parameters */     
#define     FD_WEAK_IDREF_LT    (float)(-10.5)   //-3
#define     FD_WEAK_IDREF_FILT_CONST    200

/** Stall Detection Parameters */
#undef     STALL_DETECT_ENABLE
    
#define     STALL_DETECT_EMAG_LIMIT             Q15(0.4)
#define     STALL_DETECT_EMAG_COUNT_LIMIT       40
#define     STALL_DETECT_EMAG_FILT_CONST        600
#define     STALL_DETECT_CURRENT_LIMIT          (float)(8.75)
#define     STALL_DETECT_CURRENT_COUNT_LIMIT    5

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif	/* end of __MC1_USER_PARAMS_H */
