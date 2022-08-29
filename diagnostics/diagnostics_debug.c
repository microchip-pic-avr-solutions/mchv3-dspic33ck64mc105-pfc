/**
 * diagnostics.c
 * 
 * Diagnostics code
 * 
 * Component: diagnostics
 */

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


#include "uart1.h"
#include <stdint.h>
#include "uart2.h"
#include "mc1_init.h"

extern MC1APP_DATA_T mc1;
#define X2C_BAUDRATE_DIVIDER 107
#define buffersize 11

int16_t index_tx;

int16_t datalog1_buffer[24];

    /*
     * baud rate = 100MHz/16/(1+baudrate_divider) for highspeed = false
     * baud rate = 100MHz/4/(1+baudrate_divider) for highspeed = true
     * 
     * 6250kbaud => 0 (for DIAG_BAUDRATE_DIVIDER with highspeed = false)
     * 3125kbaud => 1
     * 2083kbaud => 2
     * 1562kbaud => 3
     *  1250kbaud => 4
     *  1041kbaud => 5
     *  892kbaud => 6
     *  781kbaud => 7
     *  694kbaud => 8
     *  625kbaud => 9
     *  568kbaud => 10
     *  520.8kbaud => 11
     *  480.76kbaud => 12
     *  446.4kbaud => 13
     *  416.6kbaud => 14
     *  390.62kbaud => 15
     *  367.6kbaud => 16
     *  347.2kbaud => 17
     *  328.9kbaud => 18
     *  115.7kbaud => 54
     *   57.87kbaud => 107
     */


void load_parameters();
void DiagnosticsDebugInit(void)
{
    UART2_InterruptReceiveDisable();
    UART2_InterruptReceiveFlagClear();
    UART2_InterruptTransmitDisable();
    UART2_InterruptTransmitFlagClear();
    UART2_Initialize();
    UART2_BaudRateDividerSet(X2C_BAUDRATE_DIVIDER);
    UART2_SpeedModeStandard();
    UART2_ModuleEnable();  
}

void DiagnosticsDebugStepMain(void)
{
   
}

void DiagnosticsDebugStepIsr(void)
{
  
     unsigned short SCI_Counter = 0;
    if(U2STAHbits.UTXBE ==1)
    {    
        while (index_tx < buffersize && SCI_Counter <=3)
        {		

            U2TXREGbits.TXREG = (datalog1_buffer[index_tx]>>8)& 0xFF;
            U2TXREGbits.TXREG = (datalog1_buffer[index_tx])& 0xFF;
            SCI_Counter = SCI_Counter+1;
            index_tx=index_tx+1;
        }

        if (index_tx >= buffersize)
        {
            index_tx = 0;
            load_parameters();
        }
    }
}

void load_parameters() 
{

    datalog1_buffer[0] = 0xAA55;
    datalog1_buffer[1] = mc1.controlScheme.idq.q;//estimator.qOmegaMr;
    datalog1_buffer[2] = 2;//estimator.qVelEstim;
    datalog1_buffer[3] = 3;//ctrlParm.qVelRef;
    datalog1_buffer[4] = 4;//fan.windMill.qVelRef;
    datalog1_buffer[5] = 5;//ctrlParm.qVqRef;
    datalog1_buffer[6] = 6;//fan.windMill.qIqRef;
    datalog1_buffer[7] = 7;//vdq.q;
    datalog1_buffer[8] = 8;//fan.windMill.vqs;
    datalog1_buffer[9] = 9;//vdq.d;
    datalog1_buffer[10] = 10;//fan.windMill.vds;
//    datalog1_buffer[11] = 11;
//    datalog1_buffer[12] = 12;
//    datalog1_buffer[13] = 13;
//    datalog1_buffer[14] = 14;
//    datalog1_buffer[15] = 15;
//    datalog1_buffer[16] = 16;
//    datalog1_buffer[17] = 17;
//    datalog1_buffer[18] = 18;
//    datalog1_buffer[19] = 19;
//    datalog1_buffer[20] = 20;
//    datalog1_buffer[21] = 21;
//    datalog1_buffer[22] = 22;
//    datalog1_buffer[23] = 23;
}
/* ---------- communication primitives used by X2CScope library ---------- */



