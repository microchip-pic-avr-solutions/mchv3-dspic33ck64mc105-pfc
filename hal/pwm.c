/*******************************************************************************
  Source File for High-Resolution PWM with Fine Edge Placement Configuration

  File Name:
    pwm.c

  Summary:
    This file includes subroutine for initializing  High-Resolution PWM 

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
#include "pwm.h"

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void InitPWMGenerator1 (void);
void InitPWMGenerator2 (void);
void InitPWMGenerator3 (void);
void InitPWMGenerator4 (void);
void InitDutyPWM123Generators(void);
void InitPWMGenerators(void);   
void ChargeBootstarpCapacitors(void);
// *****************************************************************************
/* Function:
    InitPWMGenerators()

  Summary:
    Routine to initialize PWM Module for Inverters

  Description:
    Function initializes  and enable the PWM Module after configuration

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void InitPWMGenerators(void)
{

    PCLKCON      = 0x0000;
    /* PWM Clock Divider Selection bits
       0b11 = 1:16 ; 0b10 = 1:8 ;0b01 = 1:4 ; 0b00 = 1:2*/
    PCLKCONbits.DIVSEL = 0;
    /* PWM Master Clock Selection bits
       0b11 = AFPLLO ; 0b10 = FPLLO ; 0b01 = AFVCO/2 ; 0b00 = FOSC */
    PCLKCONbits.MCLKSEL = 0;
    /* Lock bit: 0 = Write-protected registers and bits are unlocked   */
    PCLKCONbits.LOCK = 0;

    /* Initialize Master Phase Register */
    MPHASE       = 0x0000;
    /* Initialize Master Duty Cycle */
    MDC          = 0x0000;
    /* Initialize Master Period Register */
    MPER         = 0X0000;
    
    /* Initialize FREQUENCY SCALE REGISTER*/
    FSCL          = 0x0000;
    /* Initialize FREQUENCY SCALING MINIMUM PERIOD REGISTER */
    FSMINPER     = 0x0000;
    /* Initialize Linear Feedback Shift Register */
    LFSR         = 0x0000;
    /* Initialize Combinational Trigger Register Low */
    CMBTRIGL     = 0x0000;
    /* Initialize Combinational Trigger Register High */
    CMBTRIGH     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 1 LOW */
    LOGCONA     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 1 HIGH */
    LOGCONB     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 2 LOW */
    LOGCONC     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 2 HIGH */
    LOGCOND     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 3 LOW */
    LOGCONE     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER 3 High */
    LOGCONF     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER A */
    PWMEVTA     = 0x0000;    
    /* PWM EVENT OUTPUT CONTROL REGISTER B */
    PWMEVTB     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER C */
    PWMEVTC     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER D */
    PWMEVTD     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER E */
    PWMEVTE     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER F */
    PWMEVTF     = 0x0000;
        
    InitPWMGenerator4 ();
    
    PG4PER = PFC_LOOPTIME_TCY;

    IFS4bits.PWM4IF = 0;
    IEC4bits.PWM4IE = 0;
    IPC17bits.PWM4IP = 7;
    
    PG4CONLbits.ON = 1;      // Enable PWM module after initializing generators
}

void InitPWMGenerator4 (void)
{

    /* Initialize PWM GENERATOR 4 CONTROL REGISTER LOW */
    PG4CONL      = 0x0000;
    /* PWM Generator 4 Enable bit : 1 = Is enabled, 0 = Is not enabled */
    /* PWM Generator is disabled prior to configuring module */
    PG4CONLbits.ON = 0;
    /* Clock Selection bits
       0b01 = Macro uses Master clock selected by the PCLKCON.MCLKSEL bits*/
    PG4CONLbits.CLKSEL = 1;
    /* PWM Mode Selection bits
       100 = Center-Aligned PWM mode(interrupt/register update once per cycle)*/
    PG4CONLbits.MODSEL = 4;
    /* Trigger Count Select bits
       000 = PWM Generator produces 1 PWM cycle after triggered */
    PG4CONLbits.TRGCNT = 0;
    
    /* Initialize PWM GENERATOR 4 CONTROL REGISTER HIGH */
    PG4CONH      = 0x0000;
    /* Master Duty Cycle Register Select bit
       1 = Macro uses the MDC register instead of PG4DC
       0 = Macro uses the PG4DC register*/
    PG4CONHbits.MDCSEL = 0;
    /* Master Period Register Select bit
       1 = Macro uses the MPER register instead of PG4PER
       0 = Macro uses the PG4PER register */
    PG4CONHbits.MPERSEL = 0;
    /* MPHSEL: Master Phase Register Select bit
       1 = Macro uses the MPHASE register instead of PG4PHASE
       0 = Macro uses the PG4PHASE register */
    PG4CONHbits.MPHSEL = 0;
    /* Master Update Enable bit
       1 = PWM Generator broadcasts software set/clear of UPDATE status bit and 
           EOC signal to other PWM Generators
       0 = PWM Generator does not broadcast UPDATE status bit or EOC signal */
    PG4CONHbits.MSTEN = 0;
    /* PWM Buffer Update Mode Selection bits 
       000 = SOC update
        Data registers at start of next PWM cycle if UPDREQ = 1.*/
	PG4CONHbits.UPDMOD = 0b000;
    /* PWM Generator Trigger Mode Selection bits
       0b00 = PWM Generator operates in Single Trigger mode */
    PG4CONHbits.TRGMOD = 0;
    /* Start of Cycle Selection bits
       0000 = Local EOC
       0100 = Trigger output selected by PG4 PGTRGSEL[2:0] bits (PGxEVTL[2:0])
       1111 = TRIG bit or PCI Sync function only (no hardware trigger source is selected)*/
    PG4CONHbits.SOCS = 0;
    
    /* Clear PWM GENERATOR 4 STATUS REGISTER*/
    PG4STAT      = 0x0000;
    /* Initialize PWM GENERATOR 4 I/O CONTROL REGISTER LOW */
    PG4IOCONL    = 0x0000;

    /* Current Limit Mode Select bit
       0 = If PCI current limit is active, then the CLDAT<1:0> bits define 
       the PWM output levels */
    PG4IOCONLbits.CLMOD = 0;
    /* Swap PWM Signals to PWM4H and PWM4L Device Pins bit 
       0 = PWM4H/L signals are mapped to their respective pins */
    PG4IOCONLbits.SWAP = 0;
    /* User Override Enable for PWM4H Pin bit
       0 = PWM Generator provides data for the PWM4H pin*/
    PG4IOCONLbits.OVRENH = 0;
    /* User Override Enable for PWM4L Pin bit
       0 = PWM Generator provides data for the PWM4L pin*/
    PG4IOCONLbits.OVRENL = 0;
    /* Data for PWM4H/PWM4L Pins if Override is Enabled bits
       If OVERENH = 1, then OVRDAT<1> provides data for PWM4H.
       If OVERENL = 1, then OVRDAT<0> provides data for PWM4L */
    PG4IOCONLbits.OVRDAT = 0;
    /* User Output Override Synchronization Control bits
       00 = User output overrides via the OVRENL/H and OVRDAT<1:0> bits are 
       synchronized to the local PWM time base (next start of cycle)*/
    PG4IOCONLbits.OSYNC = 0;
    /* Data for PWM4H/PWM4L Pins if FLT Event is Active bits
       If Fault is active, then FLTDAT<1> provides data for PWM4H.
       If Fault is active, then FLTDAT<0> provides data for PWM4L.*/
    PG4IOCONLbits.FLTDAT = 0;
    /* Data for PWM4H/PWM4L Pins if CLMT Event is Active bits
       If current limit is active, then CLDAT<1> provides data for PWM4H.
       If current limit is active, then CLDAT<0> provides data for PWM4L.*/
    PG4IOCONLbits.CLDAT = 0;
    /* Data for PWM4H/PWM4L Pins if Feed-Forward Event is Active bits
       If feed-forward is active, then FFDAT<1> provides data for PWM4H.
       If feed-forward is active, then FFDAT<0> provides data for PWM4L.*/
    PG4IOCONLbits.FFDAT = 0;
    /* Data for PWM4H/PWM4L Pins if Debug Mode is Active and PTFRZ = 1 bits
       If Debug mode is active and PTFRZ=1,then DBDAT<1> provides PWM4H data.
       If Debug mode is active and PTFRZ=1,then DBDAT<0> provides PWM4L data. */
    PG4IOCONLbits.DBDAT = 0;
    
    /* Initialize PWM GENERATOR 4 I/O CONTROL REGISTER HIGH */    
    PG4IOCONH    = 0x0000;
    /* Time Base Capture Source Selection bits
       000 = No hardware source selected for time base capture ? software only*/
    PG4IOCONHbits.CAPSRC = 0;
    /* Dead-Time Compensation Select bit 
       0 = Dead-time compensation is controlled by PCI Sync logic */
    PG4IOCONHbits.DTCMPSEL = 0;
    /* PWM Generator Output Mode Selection bits
       01 = PWM Generator outputs operate in Independent mode
       00 = PWM Generator outputs operate in Complementary mode*/
    PG4IOCONHbits.PMOD = 1;
    /* PWM4H Output Port Enable bit
       1 = PWM Generator controls the PWM4H output pin
       0 = PWM Generator does not control the PWM4H output pin */
    PG4IOCONHbits.PENH = 0;
    /* PWM4L Output Port Enable bit
       1 = PWM Generator controls the PWM4L output pin
       0 = PWM Generator does not control the PWM4L output pin */
    PG4IOCONHbits.PENL = 1;
    /* PWM4H Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG4IOCONHbits.POLH = 0;
    /* PWM4L Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG4IOCONHbits.POLL = 0;
    
    /* Initialize PWM GENERATOR 4 EVENT REGISTER LOW*/
    PG4EVTL      = 0x0000;
    /* ADC Trigger 1 Post-scaler Selection bits
       0b0000 = 1:1 
       0b0011 = 1:4 */
    PG4EVTLbits.ADTR1PS = 0b0011;
    /* ADC Trigger 1 Source is PG4TRIGC Compare Event Enable bit
       0 = PG4TRIGC register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG4EVTLbits.ADTR1EN3  = 1;
    /* ADC Trigger 1 Source is PG4TRIGB Compare Event Enable bit
       0 = PG4TRIGB register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG4EVTLbits.ADTR1EN2 = 0;
    /* ADC Trigger 1 Source is PG4TRIGA Compare Event Enable bit
       0 = PG4TRIGA register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG4EVTLbits.ADTR1EN1 = 0;
    /* Update Trigger Select bits
       01 = A write of the PG4DC register automatically sets the UPDATE bit
       00 = User must set the UPDATE bit manually*/
    PG4EVTLbits.UPDTRG = 1;
    /* PWM Generator Trigger Output Selection bits
       000 = EOC event is the PWM Generator trigger
       011 = PGxTRIGC compare event is the PWM Generator trigger*/
    PG4EVTLbits.PGTRGSEL = 0;
    
    /* Initialize PWM GENERATOR 4 EVENT REGISTER HIGH */
    PG4EVTH      = 0x0000;
    /* FLTIEN: PCI Fault Interrupt Enable bit
       0 = Fault interrupt is disabled */
    PG4EVTHbits.FLTIEN = 0;
    /* PCI Current Limit Interrupt Enable bit
       0 = Current limit interrupt is disabled */
    PG4EVTHbits.CLIEN = 0;
    /* PCI Feed-Forward Interrupt Enable bit
       0 = Feed-forward interrupt is disabled */
    PG4EVTHbits.FFIEN = 0;
    /* PCI Sync Interrupt Enable bit
       0 = Sync interrupt is disabled */
    PG4EVTHbits.SIEN = 0;
    /* Interrupt Event Selection bits
       00 = Interrupts CPU at EOC
       01 = Interrupts CPU at TRIGA compare event
       10 = Interrupts CPU at ADC Trigger 1 event
       11 = Time base interrupts are disabled */
    PG4EVTHbits.IEVTSEL = 3;
    /* ADC Trigger 2 Source is PG4TRIGC Compare Event Enable bit
       0 = PG4TRIGC register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG4EVTHbits.ADTR2EN3 = 0;
    /* ADC Trigger 2 Source is PG4TRIGB Compare Event Enable bit
       0 = PG4TRIGB register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG4EVTHbits.ADTR2EN2 = 0;
    /* ADC Trigger 2 Source is PG4TRIGA Compare Event Enable bit
       0 = PG4TRIGA register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG4EVTHbits.ADTR2EN1 = 1;
    /* ADC Trigger 1 Offset Selection bits
       00000 = No offset */
    PG4EVTHbits.ADTR1OFS = 0;
    
    /* PWM GENERATOR 4 Fault PCI REGISTER LOW */
#ifndef ENABLE_PWM_FAULT
    /* PWM GENERATOR 4 Fault PCI REGISTER LOW */
    PG4FPCIL     = 0x0000;
    /* PWM GENERATOR 4 Fault PCI REGISTER HIGH */
    PG4FPCIH     = 0x0000;
#else
       /* PWM GENERATOR 4 Fault PCI REGISTER LOW */
    PG4FPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG4FPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG4FPCILbits.TERM = 1;
    /* Acceptance Qualifier Polarity Select bit: 1 = Inverted 0 = Not inverted*/
    PG4FPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG4FPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG4FPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG4FPCILbits.PPS = 1;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG4FPCILbits.PSS = 19;
    
    /* PWM GENERATOR 4 Fault PCI REGISTER HIGH */
    PG4FPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG4FPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG4FPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG4FPCIHbits.ACP   = 0;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG4FPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG4FPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG4FPCIHbits.TQSS  = 3;
#endif
    
    /* PWM GENERATOR 4 Current Limit PCI REGISTER LOW */
    PG4CLPCIL    = 0x0000;
    /* PWM GENERATOR 4 Current Limit PCI REGISTER HIGH */
    PG4CLPCIH    = 0x0000;
    /* PWM GENERATOR 4 Feed Forward PCI REGISTER LOW */
    PG4FFPCIL    = 0x0000;
    /* PWM GENERATOR 4 Feed Forward  PCI REGISTER HIGH */
    PG4FFPCIH    = 0x0000;
    /* PWM GENERATOR 4 Sync PCI REGISTER LOW */
    PG4SPCIL     = 0x0000;
    /* PWM GENERATOR 4 Sync PCI REGISTER LOW */
    PG4SPCIH     = 0x0000;
    
    /* Initialize PWM GENERATOR 4 LEADING-EDGE BLANKING REGISTER LOW */
    PG4LEBL      = 0x0000;
    /* Initialize PWM GENERATOR 4 LEADING-EDGE BLANKING REGISTER HIGH*/
    PG4LEBH      = 0x0000;
    
    /* Initialize PWM GENERATOR 4 PHASE REGISTER */
    PG4PHASE     = 0x0000;
    /* Initialize PWM GENERATOR 4 DUTY CYCLE REGISTER */
    PG4DC        = 0X0000;
    /* Initialize PWM GENERATOR 4 DUTY CYCLE ADJUSTMENT REGISTER */
    PG4DCA       = 0x0000;
    /* Initialize PWM GENERATOR 4 PERIOD REGISTER */
    PG4PER       = 0x0000;
    /* Initialize PWM GENERATOR 4 DEAD-TIME REGISTER LOW */
    PG4DTL       = 0x0000;
    /* Initialize PWM GENERATOR 4 DEAD-TIME REGISTER HIGH */
    PG4DTH       = 0x0000;

    /* Initialize PWM GENERATOR 4 TRIGGER A REGISTER */
    PG4TRIGA     = PFC_LOOPTIME_TCY-100;
    /* Initialize PWM GENERATOR 4 TRIGGER B REGISTER */
    PG4TRIGB     = 0x0000;
    /* Initialize PWM GENERATOR 4 TRIGGER C REGISTER */
    PG4TRIGC     = 0X0000;
}
