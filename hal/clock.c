/*******************************************************************************
  Clock Configuration Routine source File

  File Name:
    clock.c

  Summary:
    This file includes subroutine for initializing Oscillator and Reference 
    Clock Output

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
#include <stdint.h>
#include "clock.h"

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits (Boot Segment Flash page address  limit)

// FSIGN

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)
#pragma config XTCFG = G3               // XT Config (24-32 MHz crystals)
#pragma config XTBST = DISABLE           // XT Boost (Boost the kick-start)

// FWDT
#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Post Scaler select bits (1:1048576)
#pragma config RCLKSEL = LPRC           // Watchdog Timer Clock Select bits (Always use LPRC)
#pragma config WINDIS = ON              // Watchdog Timer Window Enable bit (Watchdog Timer operates in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Post Scaler select bits (1:1048576)
#pragma config FWDTEN = ON_SW           // Watchdog Timer Enable bit (WDT controlled via SW, use WDTCON.ON bit)

// FPOR
#pragma config BISTDIS = DISABLED       // Memory BIST Feature Disable (mBIST on reset feature disabled)

// FICD
#pragma config ICS = PGD3               // ICD Communication Channel Select bits (Communicate on PGEC3 and PGED3)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
// FDMTIVTL
#pragma config DMTIVTL = 0x0         // Dead Man Timer Interval low word (Lower 16 bits of 32 bitDMT window interval (0-0xFFFF))

// FDMTIVTH
#pragma config DMTIVTH = 0x0        // Dead Man Timer Interval high word (Uper 16 bits of 32 bitDMT window interval (0-0xFFFF))

// FDMTCNTL
#pragma config DMTCNTL = 0x0         // Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Lower 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF))

// FDMTCNTH
#pragma config DMTCNTH = 0x0         // Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF) (Upper 16 bits of 32 bit DMT instruction count time-out value (0-0xFFFF))

// FDMT
#pragma config DMTDIS = OFF             // Dead Man Timer Disable bit (Dead Man Timer is Disabled and can be enabled by software)

// FDEVOPT
#pragma config ALTI2C1 = OFF            // Alternate I2C1 Pin bit (I2C1 mapped to SDA1/SCL1 pins)
#pragma config SPI2PIN = PPS            // SPI2 Pin Select bit (SPI2 uses I/O remap (PPS) pins)

// FALTREG
#pragma config CTXT1 = 0b101              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits (IPL Level 6)
#pragma config CTXT2 = 0b110              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits (IPL Level 7)
#pragma config CTXT3 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits (Not Assigned)
#pragma config CTXT4 = OFF              // Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits (Not Assigned)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void InitOscillator(void);
void EnableREFCLKOutput(uint16_t);
// *****************************************************************************
/* Function:
    void InitOscillator(void)

  Summary:
    Routine to configure controller oscillator

  Description:
    Function configure oscillator PLL to generate desired processor clock

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void InitOscillator(void)

{
    /*DOZEN: Doze Mode Enable bit
    0 = Processor clock and peripheral clock ratio is forced to 1:1           */
    CLKDIVbits.DOZEN = 0;
    
    /* FRCDIV<2:0>: Internal Fast RC Oscillator Post-scaler bits
       000 = FRC divided by 1 (default)                                       */
    CLKDIVbits.FRCDIV = 0;
    
        /* VCODIV<1:0>: PLL VCO Output Divider Select bits                        
     * 0b11=VCO clock; 0b10=VCO/2 clock; 0b01=VCO/3 clock ; 0b00=VCO/4 clock  */
    PLLDIVbits.VCODIV = 2;
        
     /* In this device Internal RC Oscillator is 8MHz
     * Also,In all Motor Control Development boards primary oscillator or 
     * Crystal oscillator output frequency is  8MHz
     * Hence, FPLLI (PLL Input frequency)is 8MHz in the application
     * 
     * FOSC (Oscillator output frequency),FCY (Device Operating Frequency),
     * FVCO (VCO Output Frequency )is:
     *         ( FPLLI * M)     (8 * 150)           
     * FVCO = -------------- = -----------  = 1200 MHz
     *               N1             1    
     *
     *         (FPLLI * M)     1    (8 * 150)      1     
     * FOSC = -------------- * - = -----------  * ---  = 200 MHz
     *        (N1 * N2 * N3)   2   (1 * 3 * 1)     2
     *
     * FCY  = 200 MHz / 2 =  100 MHz
     *
     * where,
     * N1 = CLKDIVbits.PLLPRE = 1 
     * N2 = PLLDIVbits.POST1DIV = 3
     * N3 = PLLDIVbits.POST2DIV = 1 
     * M = PLLFBDbits.PLLFBDIV = 150
     */
    
    /* PLL Feedback Divider bits (also denoted as ?M?, PLL multiplier)
     * M = (PLLFBDbits.PLLFBDIV)= 150                                         */
    PLLFBDbits.PLLFBDIV = 150;

    /* PLL Phase Detector I/P Divider Select bits(denoted as ?N1?,PLL pre-scaler)
     * N1 = CLKDIVbits.PLLPRE = 1                                             */
    CLKDIVbits.PLLPRE = 1;

    /* PLL Output Divider #1 Ratio bits((denoted as 'N2' or POSTDIV#1)
     * N2 = PLLDIVbits.POST1DIV = 3                                           */
    PLLDIVbits.POST1DIV = 3;
    
    /* PLL Output Divider #2 Ratio bits((denoted as 'N3' or POSTDIV#2)
     * N3 = PLLDIVbits.POST2DIV = 1                                           */
    PLLDIVbits.POST2DIV = 1;
    
    /* Initiate Clock Switch to FRC Oscillator with PLL (NOSC=0b001)
     *  NOSC = 0b001 = Fast RC Oscillator with PLL (FRCPLL)                   */
    __builtin_write_OSCCONH(0x01);

    /* Request oscillator switch to selection specified by the NOSC<2:0>bits  */
    __builtin_write_OSCCONL(OSCCON | 0x01);

    /* Wait for Clock switch to occur */
    while (OSCCONbits.OSWEN!= 0);

    /* Wait for PLL to lock */
    while (OSCCONbits.LOCK != 1);

}
// *****************************************************************************
/* Function:
    void EnableREFCLKOutput(uint16_t Divider)

  Summary:
    Routine to configure Reference Clock Output

  Description:
    Function configure Reference Clock output on to a device pin

  Precondition:
    None.

  Parameters:
    Divider - Specify Reference Clock Divider Ratio

  Returns:
    None.

  Remarks:
    Function assumes remap-able pin is configured as reference clock output
 */
void EnableREFCLKOutput(uint16_t Divider)
{
    
    if(REFOCONLbits.ROACTIVE == 0)
    {
        REFOCONHbits.RODIV = Divider;
        REFOCONLbits.ROSLP = 1;
        REFOCONLbits.ROSIDL = 1;
        REFOCONLbits.ROSEL = 1;   
        REFOCONLbits.ROOUT = 1;
        REFOCONLbits.ROEN = 1;
    }
}

/*EOF*/