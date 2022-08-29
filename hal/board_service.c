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
#include <stdint.h>
#include <stdbool.h>
#include "port_config.h"
#include "board_service.h"
#include "adc.h"
#include "pwm.h"

BUTTON_T buttonStartStop;
uint16_t boardServiceISRCounter = 0;

void BoardServiceInit(void);
void BoardServiceStepIsr(void);
void BoardService(void);
bool IsPressed_Button1(void);
void HAL_ResetPeripherals(void);
void HAL_PFCADCSwitchChannels(void);
void HAL_MCADCSwitchChannels(void);


static void ButtonGroupInitialize(void);
static void ButtonScan(BUTTON_T * ,bool);

bool IsPressed_Button1(void)
{
    if(buttonStartStop.status)
    {
        buttonStartStop.status = false;
        return true;
    }
    else
    {
        return false;
    }
}
void BoardServiceStepIsr(void)
{
    if (boardServiceISRCounter <  BOARD_SERVICE_TICK_COUNT)
    {
        boardServiceISRCounter += 1;
    }
}
void BoardService(void)
{
    if (boardServiceISRCounter ==  BOARD_SERVICE_TICK_COUNT)
    {
        /* Button scanning loop for Button 1 to start Motor A */
        ButtonScan(&buttonStartStop,BUTTON_START_STOP);
        boardServiceISRCounter = 0;
    }
}
void BoardServiceInit(void)
{
    boardServiceISRCounter = BOARD_SERVICE_TICK_COUNT;
}

void ButtonScan(BUTTON_T *pButton,bool button) 
{
    if (button == true) 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->debounceCount--;
            pButton->state = BUTTON_DEBOUNCE;
        }
    } 
    else 
    {
        if (pButton->debounceCount < BUTTON_DEBOUNCE_COUNT) 
        {
            pButton->state = BUTTON_NOT_PRESSED;
        } 
        else 
        {
            pButton->state = BUTTON_PRESSED;
            pButton->status = true;
        }
        pButton->debounceCount = 0;
    }
}
void ButtonGroupInitialize(void)
{
    buttonStartStop.state = BUTTON_NOT_PRESSED;
    buttonStartStop.debounceCount = 0;
    buttonStartStop.state = false;
}
// *****************************************************************************
/* Function:
    Init_Peripherals()

  Summary:
    Routine initializes controller peripherals

  Description:
    Routine to initialize Peripherals used for Inverter Control

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void HAL_InitPeripherals(void)
{                
    InitializeADCs();
    
    InitPWMGenerators();
    
    TIMER1_Initialize();
    TIMER1_InputClockSet();
    TIMER1_PeriodSet(TIMER1_PERIOD_COUNT);
    TIMER1_InterruptPrioritySet(5);
    TIMER1_InterruptFlagClear();
    TIMER1_InterruptEnable(); 
    TIMER1_ModuleStart();
}
void HAL_ResetPeripherals(void)
{
    uint16_t adcBuffer = 0; 
    adcBuffer = MC1_ClearADCIF_ReadADCBUF();
    MC1_ClearADCIF();
    MC1_EnableADCInterrupt();
    HAL_MC1PWMDisableOutputs();
}


void HAL_MC1PWMEnableOutputs(void)
{
    /* Set PWM Duty Cycles */
    MC1_PWM_PDC3 = 0;
    MC1_PWM_PDC2 = 0;
    MC1_PWM_PDC1 = 0;
    
    // Enable PWMs only on PWMxL ,to charge bootstrap capacitors initially.
    // Hence PWMxH is over-ridden to "LOW"

    PG3IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM3H pin
    PG2IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM2H pin
    PG1IOCONLbits.OVRENH = 0;  // 0 = PWM generator provides data for PWM1H pin

    PG3IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM3L pin
    PG2IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM2L pin
    PG1IOCONLbits.OVRENL = 0;  // 0 = PWM generator provides data for PWM1L pin
}
void HAL_MC1PWMDisableOutputs(void)
{
    /* Set PWM Duty Cycles */
    MC1_PWM_PDC3 = 0;
    MC1_PWM_PDC2 = 0;
    MC1_PWM_PDC1 = 0;
    
    PG3IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM3H,L, if Override is Enabled
    PG2IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM2H,L, if Override is Enabled
    PG1IOCONLbits.OVRDAT = 0;  // 0b00 = State for PWM1H,L, if Override is Enabled

    PG3IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM3H
    PG2IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM2H
    PG1IOCONLbits.OVRENH = 1;  // 1 = OVRDAT<1> provides data for output on PWM1H

    PG3IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM3L
    PG2IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM2L
    PG1IOCONLbits.OVRENL = 1;  // 1 = OVRDAT<0> provides data for output on PWM1L
}
/**
 * Writes three unique duty cycle values to the PWM duty cycle registers
 * corresponding to Motor #1.
 * Summary: Writes to the PWM duty cycle registers corresponding to Motor #1.
 * @param pdc Pointer to the array that holds duty cycle values
 * @example
 * <code>
 * HAL_PwmSetDutyCycles_Motor1(&pdcMotor1);
 * </code>
 */
void HAL_MC1PWMSetDutyCycles(MC_DUTYCYCLEOUT_T *pdc)
{
    if(pdc->dutycycle3 < MIN_DUTY)
    {
        pdc->dutycycle3 = MIN_DUTY;
    }
    if(pdc->dutycycle2 < MIN_DUTY)
    {
        pdc->dutycycle2 = MIN_DUTY;
    }
    if(pdc->dutycycle1 < MIN_DUTY)
    {
        pdc->dutycycle1 = MIN_DUTY;
    }
    
    MC1_PWM_PDC3 = pdc->dutycycle3;
    MC1_PWM_PDC2 = pdc->dutycycle2;
    MC1_PWM_PDC1 = pdc->dutycycle1;
}
/**
 * Writes three unique duty cycle values to the PWM duty cycle registers
 * corresponding to Motor #1.
 * Summary: Writes to the PWM duty cycle registers corresponding to Motor #1.
 * @param pdc Pointer to the array that holds duty cycle values
 * @example
 * <code>
 * HAL_PwmSetDutyCycles_Motor1(&pdcMotor1);
 * </code>
 */

/**
 * Writes three unique duty cycle values to the PWM duty cycle registers
 * corresponding to Motor #1.
 * Summary: Writes to the PWM duty cycle registers corresponding to Motor #1.
 * @param pdc Pointer to the array that holds duty cycle values
 * @example
 * <code>
 * HAL_TrapHandler();
 * </code>
 */
void HAL_TrapHandler(void)
{
    HAL_MC1PWMDisableOutputs();
    while (1)
    {
        Nop();
        Nop();
        Nop();
    }
}

void HAL_MC1MotorInputsRead(MCAPP_MEASURE_T *pMotorInputs)
{   

    
    pMotorInputs->measureCurrent.Ia = MC1_ADCBUF_IPHASE1;
    pMotorInputs->measureCurrent.Ib = MC1_ADCBUF_IPHASE2;
//    pMotorInputs->measurePot = MC1_ADCBUF_POT;
    pMotorInputs->measureVdc.value = MC_ADCBUF_VDC;
    
   
}


void HAL_MC1SetVoltageVector(int16_t vector)
{
    /* Overrides PWM based on vector number in the order of c-b-a */
    
    switch(vector)
    {
        case 0:
            /* c-b-a :: 0-0-0 */
            PG3IOCONL = PWM_BOT_ON;
            PG2IOCONL = PWM_BOT_ON;
            PG1IOCONL = PWM_BOT_ON;
        break;
        
        case 1:
            /* c-b-a :: 0-0-1 */
            PG3IOCONL = PWM_BOT_ON;
            PG2IOCONL = PWM_BOT_ON;
            PG1IOCONL = PWM_TOP_ON;
        break;
        
        case 2:
            /* c-b-a :: 0-1-1 */
            PG3IOCONL = PWM_BOT_ON;
            PG2IOCONL = PWM_TOP_ON;
            PG1IOCONL = PWM_TOP_ON;
        break;
        
        case 3:
            /* c-b-a :: 0-1-0 */
            PG3IOCONL = PWM_BOT_ON;
            PG2IOCONL = PWM_TOP_ON;
            PG1IOCONL = PWM_BOT_ON;
        break;
        
        case 4:
            /* c-b-a :: 1-1-0 */
            PG3IOCONL = PWM_TOP_ON;
            PG2IOCONL = PWM_TOP_ON;
            PG1IOCONL = PWM_BOT_ON;
        break;
        
        case 5:
            /* c-b-a :: 1-0-0 */
            PG3IOCONL = PWM_TOP_ON;
            PG2IOCONL = PWM_BOT_ON;
            PG1IOCONL = PWM_BOT_ON;
        break;
        
        case 6:
             /* c-b-a :: 1-0-1 */
            PG3IOCONL = PWM_TOP_ON;
            PG2IOCONL = PWM_BOT_ON;
            PG1IOCONL = PWM_TOP_ON;
        break;
        
        case 7:
            /* c-b-a :: 1-1-1 */
            PG3IOCONL = PWM_TOP_ON;
            PG2IOCONL = PWM_TOP_ON;
            PG1IOCONL = PWM_TOP_ON;
        break;

        default:
            vector = 0;
        break;
    }
}
//void HAL_PFCADCSwitchChannels(void)
//{
//    ADCON4Hbits.C1CHS = 0;
//    ADTRIG0Lbits.TRGSRC1 = 0x4;
//}
//
//void HAL_MCADCSwitchChannels(void)
//{
//    ADCON4Hbits.C1CHS = 1;
//    ADTRIG0Lbits.TRGSRC1 = 0b01011;
//}

void EnablePFCPWMOutputs(void)
{    
    // 0 = PWM Generator provides data for the PWM4L pin
    PG4IOCONLbits.OVRENL = 0;  
}
void DisablePFCPWMOutputs(void)
{
    /** Set Override Data on all PWM outputs */
    // 0b00 = State for PWM4L, if Override is Enabled
    PG4IOCONLbits.OVRDAT = 0;
    
    PG4IOCONLbits.OVRENL = 1; 
        
}