/*******************************************************************************
  Input / Output Port COnfiguration Routine source File

  File Name:
    port_config.c

  Summary:
    This file includes subroutine for initializing GPIO pins as analog/digital,
    input or output etc. Also to PPS functionality to Remap-able input or output 
    pins

  Description:
    Definitions in the file are for dsPIC33CK64MC105 MC PIM plugged onto
    Motor Control Development board from Microchip
 
*******************************************************************************/
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <xc.h>
#include "port_config.h"
#include "pim_select.h"

// *****************************************************************************
/* Function:
    SetupGPIOPorts()

  Summary:
    Routine to set-up GPIO ports

  Description:
    Function initializes GPIO pins for input or output ports,analog/digital pins,
    remap the peripheral functions to desires RPx pins.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void SetupGPIOPorts(void)
{
    // Reset all PORTx register (all inputs)
    #ifdef TRISA
        TRISA = 0xFFFF;
        LATA  = 0x0000;
    #endif
    #ifdef ANSELA
        ANSELA = 0x0000;
    #endif

    #ifdef TRISB
        TRISB = 0xFFFF;
        LATB  = 0x0000;
    #endif
    #ifdef ANSELB
        ANSELB = 0x0000;
    #endif

    #ifdef TRISC
        TRISC = 0xFFFF;
        LATC  = 0x0000;
    #endif
    #ifdef ANSELC
        ANSELC = 0x0000;
    #endif

    #ifdef TRISD
        TRISD = 0xFFFF;
        LATD  = 0x0000;
    #endif
    #ifdef ANSELD
        ANSELD = 0x0000;
    #endif

    #ifdef TRISE
        TRISE = 0xFFFF;
        LATE  = 0x0000;
    #endif
    #ifdef ANSELE
        ANSELE = 0x0000;
    #endif

    MapGPIOHWFunction();

    return;
}
// *****************************************************************************
/* Function:
    Map_GPIO_HW_Function()

  Summary:
    Routine to setup GPIO pin used as input/output analog/digital etc

  Description:
    Function initializes GPIO pins as input or output port pins,analog/digital 
    pins,remap the peripheral functions to desires RPx pins.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void MapGPIOHWFunction(void)
{
    
    /* ANALOG SIGNALS */

    // Configure Port pins for Motor Current Sensing
    
#ifdef INTERNAL_OPAMP_PIM
    // Ib Out
    ANSELBbits.ANSELB2 = 1;
    TRISBbits.TRISB2 = 1;   // Pin 25: OA2OUT/AN1/AN7/CMP1D/RP34/INT0/RB2
    
    //Ib- PIM:66
    ANSELBbits.ANSELB3 = 1;
    TRISBbits.TRISB3 = 1;   //Pin 26: PGD2/OA2IN-/AN8/RP35/RB3
    
    //Ib+ PIM:73
    ANSELBbits.ANSELB4 = 1;
    TRISBbits.TRISB4 = 1;   //Pin 27: PGC2/OA2IN+/RP36/RB4
    
    //Ia Out
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 1;   //Pin 08: OA1OUT/AN0/CMP1A/IBIAS0/RA0
    
    //Ia- PIM:66
    ANSELAbits.ANSELA1 = 1;
    TRISAbits.TRISA1 = 1;   //Pin 09: OA1IN-/RA1
    
    //Ia+ PIM:74
    ANSELAbits.ANSELA2 = 1;
    TRISAbits.TRISA2 = 1;   //Pin 10: OA1IN+/AN9/RA2
    
    //Op-Amp Configuration
    AMPCON1Hbits.NCHDIS2 = 0;    //Wide input range for Op Amp #2
    AMPCON1Lbits.AMPEN2 = 1;     //Enables Op Amp #2
      
    AMPCON1Hbits.NCHDIS1 = 0;    //Wide input range for Op Amp #1
    AMPCON1Lbits.AMPEN1 = 1;     //Enables Op Amp #1
    
    AMPCON1Lbits.AMPON = 1;      //Enables op amp modules if their respective AMPENx bits are also asserted
    
#else
    //Amplified Ib PIM:21
    ANSELBbits.ANSELB2 = 1;
    TRISBbits.TRISB2 = 1;   // Pin 25: OA2OUT/AN1/AN7/CMP1D/RP34/INT0/RB2
    
    //Amplified Ia PIM:22
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 1;   //Pin 08: OA1OUT/AN0/CMP1A/IBIAS0/RA0
    
#endif
    
    // Potentiometer #1 input - used as Speed Reference
    // POT1 : PIM #32
    ANSELCbits.ANSELC3 = 1;
    TRISCbits.TRISC3 = 1;   // PIN20: AN15/IBIAS2/RP51/RC3
    
    //IPFC : PIM#25
    ANSELAbits.ANSELA3 = 1;
    TRISAbits.TRISA3 = 1;//PIN11: DACOUT/AN3/CMP1C/RA3
    
    //Vac: PIM#24
    ANSELBbits.ANSELB7 = 1;
    TRISBbits.TRISB7 = 1;//PIN35 : TDO/AN2/RP39/RB7
    
    //vdc: PIM#35
    ANSELCbits.ANSELC0 = 1;
    TRISCbits.TRISC0 = 1;//PIN7 :AN12/RP48/RC0
    
    /* Digital SIGNALS */   
    // DIGITAL INPUT/OUTPUT PINS
    
    // Inverter Control - PWM Outputs
    // PWM1L : PIM #93  RP47/PWM1L/RB15
    // PWM1H : PIM #94  RP46/PWM1H/RB14
    // PWM2L : PIM #98  RP45/PWM2L/RB13
    // PWM2H : PIM #99  TDI/RP44/PWM2H/RB12
    // PWM3L : PIM #100 TCK/RP43/PWM3L/RB11
    // PWM3H : PIM #03  TMS/RP42/PWM3H/RB10
    TRISBbits.TRISB14 = 0 ;          
    TRISBbits.TRISB15 = 0 ;         
    TRISBbits.TRISB12 = 0 ;          
    TRISBbits.TRISB13 = 0 ;           
    TRISBbits.TRISB10 = 0 ;          
    TRISBbits.TRISB11 = 0 ;    
    
    //PFC_PWM : PIM #78 RP58/RC10 
    _RP58R  = 0b100011 ; //  RP58 pin to PWM4L function (ie. 35)
    
    //PFC Enable PIN
    TRISCbits.TRISC6 = 0;  //PIM 19, PIN 17 : IBIAS1/RP54/RC6
            
    // FAULT Pins
    // Motor Control FAULT : PIM #18
    TRISDbits.TRISD8 = 1;           // RP72/PCI19/RD8

    // Debug LEDs
    // LED2 : PIM #01
    TRISCbits.TRISC13 = 0;           // PIN:4 - RC13
    // LED1 : PIM #60
    TRISCbits.TRISC12 = 0;           // PIN:3 - RC12

    // Push button Switches            
    // Push Button : PIM #68
    TRISCbits.TRISC9 = 1;   // PIN29: RP57/ASCL1/SDI2/RC9   
    
	/** Diagnostic Interface for MCLV-2,MCHV-2/3,LVMCDB etc.
        Re-map UART Channels to the device pins connected to the following 
        PIM pins on the Motor Control Development Boards .
        UART_RX : PIM #49 (Input)
        UART_TX : PIM #50 (Output)   */
    _U1RXR = 56;
    _RP65R = 0b000001;
}