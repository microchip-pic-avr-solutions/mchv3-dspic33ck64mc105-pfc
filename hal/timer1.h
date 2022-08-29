// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * timer1.h
 *
 * This header file lists interface functions - to configure and enable TIMER1 
 * module and its features
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

#ifndef __TIMER1_H
#define __TIMER1_H

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
    
#ifdef __XC16__  // See comments at the top of this header file
    #include <xc.h>
#endif // __XC16__

#include <stdint.h>
#include <stdbool.h>
#include "clock.h"

// </editor-fold> 

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// <editor-fold defaultstate="expanded" desc="DEFINITIONS or MACROS ">

#define TIMER1_CLOCK            FCY 
#define TIMER1_CLOCK_PRESCALER  8
#define TIMER1_PERIOD_uSec      100.0
        
#define TIMER1_CLOCK_SCALED     (TIMER1_CLOCK/(TIMER1_CLOCK_PRESCALER*1000.0*1000.0))
        
#define TIMER1_PERIOD_COUNT  	(uint16_t)((TIMER1_CLOCK_SCALED * TIMER1_PERIOD_uSec)-1)          
        
// </editor-fold>    
        
// <editor-fold defaultstate="expanded" desc="TYPE DEFINITIONS ">  
        
/** TIMER1 Clock Pre-scalers */
typedef enum tagTIMER1_CLOCK_PRESCALER
{ 
    /** TCKPS<1:0>: Timer1 Input Clock Pre-scale Select bits
        0b11 = 1:256 , 0b10 = 1:64 ,0b01 = 1:8 0b00 = 1:1                     */
    TIMER1_CLOCK_PRESCALER_256    = 3,
    TIMER1_CLOCK_PRESCALER_64     = 2,
    TIMER1_CLOCK_PRESCALER_8      = 1,
    TIMER1_CLOCK_PRESCALER_1      = 0,
}TIMER1_CLOCK_PRESCALER_TYPE;

// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
             
extern void TIMER1_Initialize(void);
    
/**
 * Clears Timer1 interrupt request flag.
 * Summary: Clears Timer1 interrupt request flag.
 * @example
 * <code>
 * TIMER1_InterruptFlagClear();
 * </code>
 */
inline static void TIMER1_InterruptFlagClear(void) {_T1IF = 0; }

/**
 * Enable Timer1 interrupt.
 * Summary: Enable Timer1 interrupt.
 * @example
 * <code>
 * TIMER1_InterruptEnable();
 * </code>
 */
inline static void TIMER1_InterruptEnable (void) {_T1IE = 1; }
/**
 * Disable Timer1 interrupt.
 * Summary: Disable Timer1 interrupt.
 * @example
 * <code>
 * TIMER1_InterruptDisable();
 * </code>
 */
inline static void TIMER1_InterruptDisable (void) {_T1IE = 0; }
/**
 * Sets the priority level for Timer1 interrupt.
 * @param priorityValue desired priority level between 0 and 7
 * @example
 * <code>
 * TIMER1_InterruptPrioritySet(5);
 * </code>
 */
inline static void TIMER1_InterruptPrioritySet(uint16_t priorityValue)
{
    _T1IP = 0x7&priorityValue;
}
/**
 * Starts Timer1 module.
 * Summary: Starts Timer1 module.
 * @example
 * <code>
 * TIMER1_ModuleStart();
 * </code>
 */
inline static void TIMER1_ModuleStart(void) 
{
    T1CONbits.TON = 1;  
}

/**
 * Stops Timer1 module.
 * Summary: Stops Timer1 module.
 * @example
 * <code>
 * Timer1_ModuleStop();
 * </code>
 */
inline static void TIMER1_ModuleStop(void) 
{
    T1CONbits.TON = 0;  
}

/**
 * Sets the TImer1 Input Clock Select bits.
 * @example
 * <code>
 * TIMER1_InputClockSet();
 * </code>
 */
inline static void TIMER1_InputClockSet(void)
{
    T1CONbits.TSYNC = 0;
    T1CONbits.TCS = 0;
    
    if(TIMER1_CLOCK_PRESCALER == 256)
    {
        T1CONbits.TCKPS = TIMER1_CLOCK_PRESCALER_256;
    }
    else if(TIMER1_CLOCK_PRESCALER == 64)
    {
        T1CONbits.TCKPS = TIMER1_CLOCK_PRESCALER_64;
    }
    else if(TIMER1_CLOCK_PRESCALER == 8)
    {
        T1CONbits.TCKPS = TIMER1_CLOCK_PRESCALER_8;
    }
    else if(TIMER1_CLOCK_PRESCALER == 1)
    {
        T1CONbits.TCKPS = TIMER1_CLOCK_PRESCALER_1;
    }
    
}
/**
 * Sets the TImer1 Period.
 * @param period  provide desired period
 * @example
 * <code>
 * TIMER1_PeriodSet(1000);
 * </code>
 */
inline static void TIMER1_PeriodSet(uint16_t period)
{
    PR1 = period;
}
/**
 * Read the Timer1 Counter .
 * @param None.
 * @example
 * <code>
 * counter = TIMER1_CounterRead();
 * </code>
 */
inline static uint16_t TIMER1_CounterRead(void)
{
    return TMR1;
}
/**
 * Clear the Timer1 Counter .
 * @param None.
 * @example
 * <code>
 * TIMER1_CounterClear();
 * </code>
 */
inline static void TIMER1_CounterClear(void)
{
    TMR1 = 0;
}

// </editor-fold> 

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of __TIMER1_H
    