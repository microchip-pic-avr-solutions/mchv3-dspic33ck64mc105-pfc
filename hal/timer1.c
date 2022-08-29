// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * timer1.c
 *
 * This file includes subroutine to configure TIMER1 Module
 * 
 * Definitions in this file are for dsPIC33CH128MP508.
 * 
 * Component: Master Core - HAL - TIMER1
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

// <editor-fold defaultstate="collapsed" desc="Header Files ">

#ifdef __XC16__  // See comments at the top of this header file
    #include <xc.h>
#endif // __XC16__

#include <stdint.h>
#include <stdbool.h>
#include "timer1.h"

// </editor-fold> 

// <editor-fold defaultstate="collapsed" desc="FUNCTION DECLARATIONS ">


// </editor-fold> 
 
void TIMER1_Initialize(void)
{
    /** Initialize Timer1 Control Register
        bit 15:8    TON-U-SIDL-TMWDIS TMWIP-PRWIP-TECS<1:0> 
        bit 7:0     TGATE-U-TCKPS<1:0> U-TSYNC-TCS-U                          */
    T1CON = 0;
    
    /** Timer1 On bit  1 = Starts 16bit Timer1, 0 = Stops 16bit Timer1. */
    T1CONbits.TON = 0;
    /** Timer1 Stop in Idle Mode bit
        1 = Discontinues module operation when device enters Idle mode
        0 = Continues module operation in Idle mode */
    T1CONbits.TSIDL = 0;
    
    /** TGATE: Timer1 Gated Time Accumulation Enable bit
        When TCS = 1: This bit is ignored.
        When TCS = 0:
        1 = Gated time accumulation is enabled
        0 = Gated time accumulation is disabled  */
    T1CONbits.TGATE = 0;
    
    /** TCKPS<1:0>: Timer1 Input Clock Prescale Select bits
        0b11 = 1:256 , 0b10 = 1:64 ,0b01 = 1:8 0b00 = 1:1                     */
    T1CONbits.TCKPS = 0; 
    
    /** Timer1 External Clock Input Synchronization Select bit(1)
        When TCS = 1:
        1 = Synchronizes the External Clock input
        0 = Does not synchronize the External Clock input
        When TCS = 0: This bit is ignored.                                    */
    T1CONbits.TCS = 0; 

    /** Timer1 Clock Source Select 
        1 = External Clock source selected by TECS<1:0>
        0 = Internal peripheral clock (FP) */
    T1CONbits.TSYNC = 0;

    PR1 = 0; 
    TMR1 = 0;

    TIMER1_InterruptDisable();
    TIMER1_InterruptFlagClear();
}

