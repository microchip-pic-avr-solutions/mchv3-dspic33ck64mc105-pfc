// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc_app_types.h
 *
 * @brief This module initializes data structure variable type definitions of 
 * data structure
 * 
 * Component: MOTOR CONTROL APPLICATION
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

#ifndef MC_APP_TYPES_H
#define	MC_APP_TYPES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// <editor-fold defaultstate="expanded" desc="ENUMERATED CONSTANTS ">

typedef enum
{
    MCAPP_INIT = 0,                     /* Initialize Run time parameters */
    MCAPP_CMD_WAIT = 1,                 /* Wait for Run command */
    MCAPP_OFFSET = 2,                   /* Measure current offsets */
    MCAPP_LOAD_START_READY_CHECK = 3,   /* Wait for load to be ready to start */
    MCAPP_RUN = 4,                      /* Run the motor */
    MCAPP_LOAD_STOP_READY_CHECK = 5,    /* Wait for load to be ready to stop */
    MCAPP_STOP = 6,                     /* Stop the motor */
    MCAPP_FAULT = 7,                    /* Motor is in Fault mode */

}MCAPP_STATE_T;


// </editor-fold>

#ifdef	__cplusplus
}
#endif

#endif	/* MC_APP_TYPES_H */

