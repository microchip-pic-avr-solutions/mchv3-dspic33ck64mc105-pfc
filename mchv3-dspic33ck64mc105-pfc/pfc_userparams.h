/**
 * pfc_userparams.h
 *
 * This file has parameter definitions required for PFC implementation
 * 
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
#ifndef __PFC_USERPARAMS_H
#define __PFC_USERPARAMS_H
    
#ifdef __XC16__  // See comments at the top of this header file
    #include <xc.h>
#endif // __XC16__

#include <stdint.h>
#include <stdbool.h>
#include "pfc_general.h"
#include "hal/pwm.h"

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

/** Define to Enable PFC, to disable PFC make it undef */        
#define ENABLE_PFC
        
#ifndef ENABLE_PFC
    #define DEBUG_PFC_DUTY    0
#endif

/** When defined, operates in Power Reference Control. 
   That is the Voltage PI Output correspond to the input power. */
#define PFC_POWER_CONTROL       
/** Define PFC Input AC Voltage Frequency*/
#define PFC_INPUT_FREQUENCY             50 
/** Define Sample numbers for DC Bus Voltage Average
    In this case, 128 samples or 2^7 samples are taken and avearaged.
    The scaler is choosen as power of 2 ,such that shifiting is done 
    to calcualte average from the sum of samples.  */        
#define PFC_AVG_SCALER                  7 //2^7 = 128 samples average 
/** Counter for RMS Calculation for half cycle of Input Voltage*/       
#define PFC_RMS_SQUARE_COUNTMAX         (uint16_t)(PFC_PWMFREQUENCY_HZ/(2*PFC_INPUT_FREQUENCY))      
/** Base value of the voltage is calculated as follows:
    For a maximum ADC input of 3.3V, and with resistor divider gain
                        = 2.2kOhm/(300kOhm+2.2kOhm) = 0.00727995. 
    Then,        Vinadc = Vbase* gain
                 3.3V   = Vbase*0.00727995
    Therefore, maximum voltage or base voltage = 453V */        
#define PFC_VOLTAGE_BASE                453.0
/** Base value of the inductor current is calculated as follows:
    For a maximum ADC input of 1.65V, and with Rshunt of 0.015 Ohm and with
    Operational amplifier gain = 3.1kOhm/(560ohm+39ohm) = 5.175. 
    We know that Vinadc = Ishunt*Rshunt*gain
                 1.65V  = Ishunt*0.015*5.175 
    Therefore, maximum current or base current = 21.3 A */
#define PFC_INPUT_MAX_CURRENT           21.3
                
/** PFC Fault Limits - Input Voltage,Input Current and Output Voltages */
/** PFC Input Over Current Limit in Arms */
#define PFC_INPUT_OVER_CURRENT          12.0
        
/** Specify PFC Input Voltage Range in which PFC Control will start executing */ 
/** PFC Input Over Voltage Lower Limit  in Vrms  */
#define PFC_INPUT_OVER_VOLTAGE_LO       240.0
/** PFC Input Over Voltage Upper Limit  in Vrms  */
#define PFC_INPUT_OVER_VOLTAGE_HI       255.0
/** PFC Input Under Voltage Lower Limit in Vrms */
#define PFC_INPUT_UNDER_VOLTAGE_LO      110.0
/** PFC Input Under Voltage Upper Limit in Vrms */
#define PFC_INPUT_UNDER_VOLTAGE_HI      130.0	
        
/** PFC DC Over Voltage Limit in Volts */
#define PFC_OUTPUT_OVER_VOLTAGE         410.0
/** PFC DC Over Voltage Limit in Volts */
#define PFC_OUTPUT_UNDER_VOLTAGE        310.0
     
/** Specify PFC Output Voltage Reference in Volts */
#define PFC_OUPUT_VOLTAGE_NOMINAL       380
        
/** Soft start ramp rate and ramp count .This is specified at the rate 
of PFC Control loop execution rate  **/
#define RAMP_COUNT                      1
#define RAMP_RATE                       20

/** Define Minimum PFC Voltage Control Output at which PWM duty is applied to 
    the boost power converter. This implements burst control at very low load.*/        
#define PFC_MIN_CURRENTREF_PEAK_Q15     100        
/** Voltage loop execution rate . This is specified in integral number of 
    PFC current control loop execution rate. 
    In this case , PFC control loop frequency is 80 kHz . Then voltage loop is 
    executed at 80kHz/ VOLTAGE_LOOP_EXE_RATE = 80kHz/16 = 5kHz  */   
#define VOLTAGE_LOOP_EXE_RATE           16
           
/** KMUL is used as a scaling constant    
    KMUL is calculated such that the current reference value
    equals to its maximum value at minimum line voltage.        
    Therefore,it is calculated as :            
    KMUL =  [Vacpk(min)/(2*Vac_base)]*32767 */ 
#define KMUL                            7161
    
/** PFC PI parameter */      
/** PFC Current loop Coefficients */
#define KP_I                            Q15(0.7044)
#define KI_I                            Q15(0.177)
#define KP_I_SCALE                      2
#define KI_I_SCALE                      0
#define KI_I_INTGRAL_OUT_MAX            Q15(PFC_MAX_DUTY_PU)
        
/** Voltage  loop Coefficients */
#define KP_V                            Q15(0.9040)
#define KI_V                            Q15(0.0045)  
#define KP_V_SCALE                      2
#define KI_V_SCALE                      0  

/** Calculate nominal values of PFC output voltage operating range*/  
#define PFC_OUPUT_VOLTAGE_REFERENCE             Q15(NORM_VALUE(PFC_OUPUT_VOLTAGE_NOMINAL,PFC_VOLTAGE_BASE))
 
/** Normalized RMS square: Input under voltage lower limit in Vrms^2*/
#define PFC_INPUT_UNDER_VOLTAGE_LO_RMS_SQUARE   NORM_RMS_SQUARE(PFC_INPUT_UNDER_VOLTAGE_LO,PFC_VOLTAGE_BASE)       
/** Normalized RMS square: Input under voltage upper limit in Vrms^2*/
#define PFC_INPUT_UNDER_VOLTAGE_HI_RMS_SQUARE   NORM_RMS_SQUARE(PFC_INPUT_UNDER_VOLTAGE_HI,PFC_VOLTAGE_BASE)        
/** Normalized RMS square: Input over voltage lower limit in Vrms^2*/
#define PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_LO    NORM_RMS_SQUARE(PFC_INPUT_OVER_VOLTAGE_LO,PFC_VOLTAGE_BASE)
/** Normalized RMS square: Input over voltage upper limit in Vrms^2*/
#define PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_HI    NORM_RMS_SQUARE(PFC_INPUT_OVER_VOLTAGE_HI,PFC_VOLTAGE_BASE)           
/** Normalize PFC output over voltage with PFC full scale DC output voltage */
#define PFC_OUTPUT_OVER_VOLTAGE_NORMALIZED      NORM_VALUE(PFC_OUTPUT_OVER_VOLTAGE,PFC_VOLTAGE_BASE)
/**  Normalize PFC output under voltage with PFC full scale DC output voltage */
#define PFC_OUTPUT_UNDER_VOLTAGE_NORMALIZED     NORM_VALUE(PFC_OUTPUT_UNDER_VOLTAGE,PFC_VOLTAGE_BASE)
    
/** Fault limits are converted to q15 Representation and used by the firmware */
#define PFC_OUTPUT_OVER_VOLTAGE_LIMIT           Q15(PFC_OUTPUT_OVER_VOLTAGE_NORMALIZED)
#define PFC_INPUT_UNDER_VOLTAGE_LIMIT_LO        Q15(PFC_INPUT_UNDER_VOLTAGE_LO_RMS_SQUARE)  
#define PFC_INPUT_UNDER_VOLTAGE_LIMIT_HI        Q15(PFC_INPUT_UNDER_VOLTAGE_HI_RMS_SQUARE)
#define PFC_INPUT_OVER_VOLTAGE_LIMIT_LO         Q15(PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_LO)
#define PFC_INPUT_OVER_VOLTAGE_LIMIT_HI         Q15(PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_HI)    
               
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of __PFC_PARAMS_H
    