
/**
 * pfc_userparams.h
 *
 * This file has parameter definitions required for PFC implementation
 * 

 * 
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

/* Define to Enable PFC, to disable PFC make it undef */        
#define ENABLE_PFC
        
#ifndef ENABLE_PFC
    #define DEBUG_PFC_DUTY    0
#endif

/* Define If output of PFC Voltage  PI Control has to be Power, undef if output of 
 * PFC Voltage PI Control has to be PFC Current */
#define PFC_POWER_CONTROL
/* Define to enable ADC offset correction for PFC Current Analog Channel 
 * ,undef to disable offset Correction */
#define ENABLE_PFC_CURRENT_OFFSET_CORRECTION        
/* Define PFC Input AC Voltage Frequency*/
#define PFC_INPUT_FREQUENCY  50 
/* Define Sample numbers for DC Bus Voltage Average*/        
#define PFC_AVG_SCALER      7 //2^7 = 128 samples average
 /*Counter for RMS Calculation for half cycle of Input Voltage*/       
#define PFC_RMS_SQUARE_COUNTMAX    (uint16_t)((uint32_t)PFC_PWMFREQUENCY_HZ/(2*PFC_INPUT_FREQUENCY)) 
        
/** Base Value of Voltage for the System. This value is calculated as follows:
With the resistor divider used to measure DCBUSSENSE signal, 3.3V is generated 
when an input of 453V is present in DCBUS */
#define PFC_VOLTAGE_BASE      453.0
/** Base Value of Current for the System
     This is how this value was calculated:Diff Operational amplifier of
     3.1kOhm/(560ohm+39ohm) = 5.175. So:
     Vinadc = Ishunt*Rshunt*5.175. For a maximum ADC input of 1.65V, and 
     with Rshunt of 0.015 Ohm
     we have a maximum current = 21.3 A */
#define PFC_INPUT_MAX_CURRENT       21.3
                
/** PFC Fault Limits - Input Voltage,Input Current and Output Voltages */
/** PFC Input Over Current Limit in Arms */
#define PFC_INPUT_OVER_CURRENT  12.0
        
/** Specify PFC Input Voltage Range in which PFC Control will start executing */ 
/** PFC Input Over Voltage Lower Limit  in Vrms  */
#define PFC_INPUT_OVER_VOLTAGE_LO  245.0
/** PFC Input Over Voltage Upper Limit  in Vrms  */
#define PFC_INPUT_OVER_VOLTAGE_HI  255.0
/** PFC Input Under Voltage Lower Limit in Vrms */
#define PFC_INPUT_UNDER_VOLTAGE_LO 90.0
/** PFC Input Under Voltage Upper Limit in Vrms */
#define PFC_INPUT_UNDER_VOLTAGE_HI   95.0	
        
/** PFC DC Over Voltage Limit in Volts */
#define PFC_OUTPUT_OVER_VOLTAGE  410.0
/** PFC DC Over Voltage Limit in Volts */
#define PFC_OUTPUT_UNDER_VOLTAGE 310.0

        
/** Specify PFC Output Voltage Reference in Volts */
#define PFC_OUPUT_VOLTAGE_NOMINAL   380
        
/** Soft start ramp rate and ramp count .This is specified at the rate 
of PFC Control loop execution rate  **/
#define RAMP_COUNT                  1
#define RAMP_RATE                   20

/*Define Minimum PFC Voltage Control Output to Turn ON PFC IGBT, 
 * This is doing to reduce switching losses when operating under very low load*/        
#define PFC_MIN_CURRENTREF_PEAK_Q15     100        
/** Voltage loop execution rate . This is specified at rate of PFC COntrol loop 
 rate . In this case , PFC control loop frequency is 80 KHz . Then voltage loop
 is executed at every 5KHz (80KHz/16 = 5KHz)  */   
#define VOLTAGE_LOOP_EXE_RATE       16
         
        
/** KMUL is used as scaling constant    
    KMUL is calculated such that the current reference value
    equals to its maximum value at minimum line voltage.
 *           
    Therefore is calculated as follows:            
    KMUL =  [Vacpk(min)/(2*Vac_base)]*32767
*/ 
#define KMUL   7161
    
/** PFC PI parameter */      
/** PFC Current loop Coefficients */
#define KP_I          Q15(0.7044)
#define KI_I          Q15(0.177)
#define KP_I_SCALE    2
#define KI_I_SCALE    0
#define KI_I_INTGRAL_OUT_MAX    Q15(PFC_MAX_DUTY_PU)
        
/** Voltage  loop Coefficients */
#define KP_V          Q15(0.9040)
#define KI_V          Q15(0.0045)  
#define KP_V_SCALE    2
#define KI_V_SCALE    0  

/** Calculate nominal values of PFC Output voltage operating Range*/  
#define PFC_OUPUT_VOLTAGE_REFERENCE             Q15(NORM_VALUE(PFC_OUPUT_VOLTAGE_NOMINAL,PFC_VOLTAGE_BASE))
#define PFC_MIN_VOLTAGE_REFERENCE_Q15           Q15((PFC_INPUT_UNDER_VOLTAGE_HI*SQR_ROOT_OF_2)/PFC_VOLTAGE_BASE)
 
/** Calculate Normalized RMS Square Value of PFC Input Under Voltage Lower Limit in Vrms^2*/
#define PFC_INPUT_UNDER_VOLTAGE_LO_RMS_SQUARE   NORM_RMS_SQUARE(PFC_INPUT_UNDER_VOLTAGE_LO,PFC_VOLTAGE_BASE)
#define PFC_INPUT_UNDER_VOLTAGE_LIMIT_LO        Q15(PFC_INPUT_UNDER_VOLTAGE_LO_RMS_SQUARE)        
/** Calculate Normalized RMS Square Value of PFC Input Under Voltage Upper Limit in Vrms^2*/
#define PFC_INPUT_UNDER_VOLTAGE_HI_RMS_SQUARE   NORM_RMS_SQUARE(PFC_INPUT_UNDER_VOLTAGE_HI,PFC_VOLTAGE_BASE)
#define PFC_INPUT_UNDER_VOLTAGE_LIMIT_HI        Q15(PFC_INPUT_UNDER_VOLTAGE_HI_RMS_SQUARE)
        
/** Calculate Normalized RMS Square Value of PFC Input Over Voltage Lower Limit in Vrms^2 */
#define PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_LO    NORM_RMS_SQUARE(PFC_INPUT_OVER_VOLTAGE_LO,PFC_VOLTAGE_BASE)
#define PFC_INPUT_OVER_VOLTAGE_LIMIT_LO         Q15(PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_LO) 
/** Calculate Normalized RMS Square Value of PFC Input Over Voltage Upper Limit in Vrms^2 */
#define PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_HI    NORM_RMS_SQUARE(PFC_INPUT_OVER_VOLTAGE_HI,PFC_VOLTAGE_BASE)
#define PFC_INPUT_OVER_VOLTAGE_LIMIT_HI         Q15(PFC_INPUT_OVER_VOLTAGE_RMS_SQUARE_HI)  
            
/** Normalized RMS Square Value of AC Input Signals */
/** Calculate Normalized RMS Square Value of Input Over Current in Arms^2 */
#define PFC_INPUT_OVER_CURRENT_RMS_SQUARE       NORM_RMS_SQUARE(PFC_INPUT_OVER_CURRENT,PFC_INPUT_MAX_CURRENT) 
/** Normalized value of DC Voltages */
/** Normalize PFC Output Over Voltage to PFC Full Scale DC Output Voltage */
#define PFC_OUTPUT_OVER_VOLTAGE_NORMALIZED      NORM_VALUE(PFC_OUTPUT_OVER_VOLTAGE,PFC_VOLTAGE_BASE)
/**  Normalize PFC Output Under Voltage to PFC Full Scale DC Output Voltage */
#define PFC_OUTPUT_UNDER_VOLTAGE_NORMALIZED     NORM_VALUE(PFC_OUTPUT_UNDER_VOLTAGE,PFC_VOLTAGE_BASE)
    
/** Fault limits are converted to q15 Representation and used by the firmware */
#define PFC_INPUT_OVER_CURRENT_LIMIT    Q15(PFC_INPUT_OVER_CURRENT_RMS_SQUARE)
#define PFC_OUTPUT_OVER_VOLTAGE_LIMIT   Q15(PFC_OUTPUT_OVER_VOLTAGE_NORMALIZED)
#define PFC_OUTPUT_UNDER_VOLTAGE_LIMIT  Q15(PFC_OUTPUT_UNDER_VOLTAGE_NORMALIZED)
                                      
#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of __PFC_PARAMS_H
    