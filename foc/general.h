// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file general.h
 *
 * @brief This file has general definitions and macros used in the application
 *        firmware.
 *
 * Component: APPLICATION
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
#ifndef __GENERAL_H
#define __GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="DEFINITIONS/MACROS ">
/* PI */
#define PI          3.1415927

/* Normalized value of P1(3.14) by 2,where PI = 32767 */
#define PI_BY_2     16384

/* Fraction of dc link voltage to set the limit of PI Output of
 * current controllers. */
#define VMAX_FLOAT                  0.9

/* Fraction of dc link voltage expressed as a squared amplitude. */
#define VMAX_SQR                    Q15(VMAX_FLOAT * VMAX_FLOAT)

/* Fraction of dc link voltage expressed as a squared amplitude. */
#define VMAX_SQR_FDWEAK             Q15(0.8)
    
/* Fraction of maximum speed acting as a buffer for overflow */
#define MAX_SPEED_FACTOR_FLOAT      0.5
#define QMAX_SPEED_FACTOR           Q15(MAX_SPEED_FACTOR_FLOAT)
    
/* Macro to convert a floating point number to Q15 format fixed point number */
#define Q15(floatValue)	\
            ((floatValue < 0.0) ? (int16_t)(32768 * (floatValue) - 0.5): \
            (int16_t)(32767 * (floatValue) + 0.5))

/* Function to calculate normalized value of a parameter */
#define NORM_VALUE(actual, base)    Q15( ((float)actual) / ((float)base) )

/* Function to calculate normalized sampling time */
#define NORM_DELTAT(period, maximumSpeed, polePairs)         \
    (int16_t)((period / 30.0) * 32768.0 * maximumSpeed * polePairs)

/* WBASE_DT_LSGAIN = pi * NORM_DETLAT */
#define WBASE_DT_LSGAIN(normDt)     (int16_t)(normDt * PI)

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif	/* end of __GENERAL_H */
