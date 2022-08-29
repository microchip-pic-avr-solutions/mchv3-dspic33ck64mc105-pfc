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
#ifndef __BOARD_SERVICE_H
#define __BOARD_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#include <xc.h>

#include "delay.h"
#include "clock.h"
#include "pwm.h"
#include "adc.h"
#include "port_config.h"
#include "timer1.h"
#include "measure.h"

#include "../motor_control/motor_control.h"

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

/* Button Scanning State

  Description:
    This structure will host parameters required by Button scan routine.
 */
typedef enum tagBUTTON_STATE
{
    BUTTON_NOT_PRESSED = 0, // wait for button to be pressed
    BUTTON_PRESSED = 1, // button was pressed, check if short press / long press
    BUTTON_DEBOUNCE = 2
} BUTTON_STATE;
    
// *****************************************************************************
/* Button data type

  Description:
    This structure will host parameters required by Button scan routine.
 */
typedef struct
{
   BUTTON_STATE state;
   uint16_t debounceCount;
   bool logicState;
   bool status;
} BUTTON_T;

/** Button De-bounce in milli Seconds */
#define     BUTTON_DEBOUNCE_COUNT       30
/** The board service Tick is set as 1 millisecond - specify the count in terms 
    PWM ISR cycles (i.e. BOARD_SERVICE_TICK_COUNT = 1 milli Second / PWM period)*/
#define     BOARD_SERVICE_TICK_COUNT    20

#define 	MC1_BEMF_SAMPLING_FACTOR    4

/** Board Parameters */
#define     MC1_PEAK_VOLTAGE            453.3  
#define     MC1_PEAK_CURRENT            16

#define     MC1_MOTOR_START_DC_VOLT     250.0
#define     MC1_MOTOR_STOP_DC_VOLT      200.0


void BoardServiceInit(void);
void BoardServiceStepIsr(void);
void BoardService(void);
bool IsPressed_Button1(void);
void HAL_InitPeripherals(void);
void HAL_ResetPeripherals(void);

void HAL_MC1PWMDisableOutputs(void);
void HAL_MC1PWMEnableOutputs(void);
void HAL_MC1PWMSetDutyCycles(MC_DUTYCYCLEOUT_T *);
void HAL_MC1MotorInputsRead(MCAPP_MEASURE_T *);
void HAL_MC1SetVoltageVector(int16_t);
extern void HAL_PFCADCSwitchChannels(void);
extern void HAL_MCADCSwitchChannels(void);



void HAL_TrapHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_SERVICE_H */
