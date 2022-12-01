// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file main.c
 *
 * @brief This is the main entry to the application.
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include <xc.h>

#include "board_service.h"

#include "diagnostics/diagnostics.h"

#include "pfc.h"


// </editor-fold>

// <editor-fold defaultstate="collapsed" desc=" Global Variables ">

// </editor-fold>

/**
* <B> Function: int main (void)  </B>
*
* @brief main() function,entry point of the application.
*
*/
int main (void)
{
    InitOscillator();
    SetupGPIOPorts();
    
#ifdef ENABLE_DIAGNOSTICS
    DiagnosticsInit();
#endif
    HAL_InitPeripherals();

    LED1 = 1;
    PFC_Initialize();
    while(1)
    {
        
#ifdef ENABLE_DIAGNOSTICS
        DiagnosticsStepMain();
#endif
        BoardService();
        if(IsPressed_Button1())
        {
            
        }
    }
}

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt (void)
{
    #ifdef ENABLE_DIAGNOSTICS
        DiagnosticsStepIsr();
    #endif
    BoardServiceStepIsr();
    TIMER1_InterruptFlagClear();
}
void __attribute__((__interrupt__,no_auto_psv)) PFC_ADCInterrupt()
{    
    /** Load ADC Buffer data to respective variables */
    /*Converting dcVoltage from 0 to 65535 to 0 to 32767*/
    pfcMeasured.dcVoltage = ADCBUF_PFC_VDC;
    
    pfcMeasured.acVoltage = ADCBUF_PFC_VAC;
        
    /** Read PFC Phase current */
    pfcMeasured.current = ADCBUF_PFC_IPH;     
            
    PFC_ControlLoopMain();
#ifdef ENABLE_PFC
    /** Load PWM Duty cycle to control PFC */
    PFC_ENABLE_SIGNAL = 1;
    PFC_PWM_PDC = pfcParam.duty;
#else
    PFC_PWM_PDC = DEBUG_PFC_DUTY;
#endif    
    ClearPFCADCIF();
}
