// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * pfc_params.h
 *
 * This file has parameter definitions required for PFC implementation
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
    
#ifdef __XC16__  // See comments at the top of this header file
    #include <xc.h>
#endif // __XC16__

#include <stdint.h>
#include <stdbool.h>
#include "libq.h"
#include "pfc.h"
#include "board_service.h"

// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="FUNCTION DECLARATIONS ">

void PFC_Initialize(void);
void PFC_ControlLoopMain(void);

static int16_t PFC_SampleCorrection(int16_t ,int16_t ,int16_t );
static void PFC_Average(PFC_AVG_T *,int16_t);
static void PFC_SquaredRMSCalculate(PFC_RMS_SQUARE_T *,int16_t);

inline static void PFC_CurrentControlLoop(void);
inline static void PFC_CurrentRefGenerate(void);

// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="VARIABLES ">

PFC_MEAS_T pfcMeasured;
PFC_AVG_T pfcVdcAVG;
PFC_RMS_SQUARE_T pfcVacRMS;
PFC_GENERAL_T pfcParam;

PFC_PI_T pfcPIVoltage;
PFC_PI_T pfcPICurrent;

PFC_CTRL_STATE_T pfcState;

volatile int16_t currentReference,boostDutyRatio;
volatile uint16_t pfcFaultStatus = 0;

// </editor-fold> 

/**
 * Function to initialize PFC related variables 
 * @param none.
 * @return none.
 * @example
 * <code>
 * status = PFC_Initialize();
 * </code>
 */
void PFC_Initialize(void)
{  
    int16_t adcDataBuffer;
    
    /* Make sure ADC does not generate interrupt while initializing parameters*/
	DisablePFCADCInterrupt();
    /** Initialize variables related to RMS calculation - VAC */      
    pfcVacRMS.sampleLimit = PFC_RMS_SQUARE_COUNTMAX;
    /** Initialize variables related to Average calculation - VDC */ 
    pfcVdcAVG.scaler = PFC_AVG_SCALER;
    pfcVdcAVG.sampleLimit = 1<<pfcVdcAVG.scaler;

/** Initialize PI controlling PFC Current Loop */    
    pfcPICurrent.kp = KP_I;
    pfcPICurrent.ki = KI_I;
    pfcPICurrent.kpScale = KP_I_SCALE;
    pfcPICurrent.kiScale = KI_I_SCALE;
    pfcPICurrent.maxOutput = INT16_MAX;
    pfcPICurrent.minOutput = 0;
    
/** Initialize PI controlling PFC Voltage Loop */    
    pfcPIVoltage.kp = KP_V;
    pfcPIVoltage.ki = KI_V;
    pfcPIVoltage.kpScale = KP_V_SCALE;
    pfcPIVoltage.kiScale = KI_V_SCALE;
    pfcPIVoltage.maxOutput = INT16_MAX;
    pfcPIVoltage.minOutput = 0;
    
    pfcState = PFC_INIT;
    pfcFaultStatus = PFC_FAULT_NONE;
    
    /* Enable ADC interrupt and begin main loop timing */
    ClearPFCADCIF();
    adcDataBuffer = ClearPFCADCIF_ReadADCBUF();
    EnablePFCADCInterrupt(); 
}

/**
 * Function to perform  - VAC RMS calculation, calculate moving average 
 * of VDC, PFC Voltage control loop,  Generate current reference, PFC  
 * Current control Loop
 * @param none.
 * @return none.
 * @example
 * <code>
 * status = PFC_ControlLoopMain();
 * </code>
 */
void PFC_ControlLoopMain(void)
{    
    /** Calculate average of PFC output voltage ( DC voltage) feedback 
    to remove line frequency ripple */
    PFC_Average(&pfcVdcAVG,pfcMeasured.dcVoltage);
    /** Calculate RMS Square of AC input voltage  */
    PFC_SquaredRMSCalculate(&pfcVacRMS,pfcMeasured.acVoltage);
    
    switch(pfcState)
    {
        case PFC_INIT:
            
            /** Initialize variables related to moving average filter - VDC */    
            pfcVdcAVG.sum = 0;
            pfcVdcAVG.samples = 0;
            pfcVacRMS.sum = 0;
            pfcVacRMS.samples = 0;
            pfcVacRMS.peak = 0;
            pfcParam.duty = 0;
            pfcPIVoltage.integralOut = 0;
            pfcPICurrent.integralOut = 0;
            PFC_MeasureCurrentInit(&pfcMeasured);
              
            pfcState = PFC_OFFSET_MEAS;
            
        break;
        case PFC_OFFSET_MEAS:
            PFC_MeasureCurrentOffset(&pfcMeasured);
            
            if(pfcMeasured.status == 1)
            {               
                /** Enable PFC Gate Driver outputs */ 
                #ifdef ENABLE_PFC
                    /** PFC_ENABLE - ON */
                    EnablePFCPWMOutputs();
                #else
                    /** PFC_ENABLE - OFF */
                    DisablePFCPWMOutputs();
                #endif
                
                /** Upon first entry into this loop, set PFC output voltage reference 
                    as measures DC voltage and enable  soft start */
                pfcPIVoltage.reference = pfcVdcAVG.output;

                pfcState = PFC_CTRL_RUN;
            }
        break;
        case PFC_CTRL_RUN:
            /*Limiting PFC Input rectified acVoltage b/w minimum and maximum Limits*/
            if (pfcMeasured.acVoltage > Q15(0.998))
            {
                pfcMeasured.acVoltage = Q15(0.998);
            }
            else if(pfcMeasured.acVoltage < 0)
            {
                pfcMeasured.acVoltage = 1;
            }

            if(pfcVdcAVG.output >= PFC_OUTPUT_OVER_VOLTAGE_LIMIT)
            {
                pfcFaultStatus += PFC_FAULT_OP_OV;
                
            }
            if(pfcVacRMS.sqrOutput < PFC_INPUT_UNDER_VOLTAGE_LIMIT_LO)
            {
                pfcFaultStatus += PFC_FAULT_IP_UV;
            }
            if(pfcVacRMS.sqrOutput >= PFC_INPUT_OVER_VOLTAGE_LIMIT_HI)
            {
                pfcFaultStatus += PFC_FAULT_IP_OV;
            }
            if(pfcFaultStatus == PFC_FAULT_NONE)
            {
                /** Perform soft start when enabled */
                if (pfcPIVoltage.reference < PFC_OUPUT_VOLTAGE_REFERENCE)
                {
                    if(pfcParam.rampRate == 0)
                    {
                        pfcPIVoltage.reference = pfcPIVoltage.reference + RAMP_COUNT;
                        pfcParam.rampRate = RAMP_RATE;
                    }
                    else
                    {
                        pfcParam.rampRate--;
                    }
                }
                else
                {
                    pfcPIVoltage.reference = PFC_OUPUT_VOLTAGE_REFERENCE; 
                }

                PFC_CurrentRefGenerate();                

                if(pfcMeasured.dcVoltage >  0)
                {
                    /** Calculate Ideal value of Boost converter duty ratio based on 
                        current value of Vdc and Vac 
                        Boost Duty Ratio = (1 - (Vac/Vdc)) */
                    boostDutyRatio = __builtin_divf(pfcMeasured.dcVoltage - 
                                pfcMeasured.acVoltage, pfcMeasured.dcVoltage) ;
                }
                
                PFC_CurrentControlLoop();
                
                if(pfcPIVoltage.output < PFC_MIN_CURRENTREF_PEAK_Q15)
                {
                    pfcParam.duty = 0;
                    pfcPICurrent.integralOut = 0;
                }
            }
            else
            {
                pfcState = PFC_FAULT; 
            }
            break;
        case PFC_FAULT:
            pfcParam.duty = 0;
            DisablePFCPWMOutputs();
            if(pfcVacRMS.sqrOutput >= PFC_INPUT_UNDER_VOLTAGE_LIMIT_HI)
            {
                pfcFaultStatus &= (~PFC_FAULT_IP_UV);
                
            }
            if(pfcVacRMS.sqrOutput < PFC_INPUT_OVER_VOLTAGE_LIMIT_LO )
            {
                pfcFaultStatus &= (~PFC_FAULT_IP_OV);
               
            }
            if(pfcFaultStatus == PFC_FAULT_NONE)
            {
                pfcPIVoltage.integralOut = 0;
                pfcPICurrent.integralOut = 0;
                pfcPIVoltage.reference = pfcVdcAVG.output;
                pfcState = PFC_CTRL_RUN;
                EnablePFCPWMOutputs();
            }
        break;
        default:
        break;   
    }
}
/**
 * Function to calculate RMS Value of an input Signal 
 * @param none.
 * @return none.
 * @example
 * <code>
 * PFC_SquaredRMSCalculate(&signal,input);
 * </code>
 */
static void PFC_SquaredRMSCalculate(PFC_RMS_SQUARE_T *pData,int16_t input)
{       
    int16_t sqrt;
    pData->sum += (int16_t) (__builtin_mulss(input,input) >> 15);
    if(pData->samples < pData->sampleLimit)
    {
       pData->samples++;
       if(input>=pData->peakcheck)
       {
           pData->peakcheck = input;
       }    
    }
    else
    {
       pData->sqrOutput = (int16_t)(__builtin_divsd(pData->sum,
               pData->sampleLimit));
       pData->samples = 0;
       pData->sum = 0;
       sqrt = _Q15sqrt(pData->sqrOutput);
       pData->peak = sqrt+ (__builtin_mulss(sqrt,13565) >> 15);
       pData->peak = pData->peakcheck;
       pData->peakcheck = 0;
    }
}
/**
 * Function to calculate moving average value of an input Signal 
 * @param pointer to Signal Data structure, Current value of signal
 * @return none.
 * @example
 * <code>
 * PFC_Average(&signal,input);
 * </code>
 */
static void PFC_Average(PFC_AVG_T *pData,int16_t input)
{
    pData->sum = pData->sum + input;
    pData->samples++;
    if(pData->samples >= pData->sampleLimit) 
    {
        pData->output = (int16_t)( __builtin_divsd(pData->sum,
                                            pData->sampleLimit));
        pData->sum = 0;
        pData->samples = 0; 
    }
}
/**
 * Function to calculate average value of current in discontinuous conduction
 * mode.
 * @param ideal Duty Ratio,actual Duty Ratio,present value of current
 * @return average current output
 * @example
 * <code>
 * current = PFC_SampleCorrection(idealDuty,actualDuty,measuredCurrent);
 * </code>
 */
static int16_t PFC_SampleCorrection(int16_t idealDuty,int16_t actualDuty,int16_t current)
{
    int16_t output = Q15(0.9999);
    
    /** Check if Ideal Duty is positive value */
    if(idealDuty > 0)
    {
        /** Calculate ratio of actual Duty and ideal Duty */
        output = __builtin_divf(actualDuty, idealDuty) ;
    }
    /** Check if ratio previous result is greater than 0 */
    if(output > 0)
    {
        output = (int16_t)((__builtin_mulss(current,output)) >> 15);
    }
    else
    {
        output = current;
    }
    output = current;
    return(output);
}
/**
 * Function to execute current control loop
 * @param none
 * @return none
 * @example
 * <code>
 * PFC_CurrentControlLoop();
 * </code>
 */
inline static void PFC_CurrentControlLoop(void)
{
#ifdef ENABLE_PFC_CURRENT_OFFSET_CORRECTION
    pfcMeasured.current = pfcMeasured.current-pfcMeasured.offsetI;
#endif
        
    /** Ensure PFC Current  is not negative.*/ 
    if (pfcMeasured.current < 0)
    {
        pfcMeasured.current  = 1;
    }
    /** Calculate average current if converter operates in discontinuous 
        conduction mode. In continuous conduction mode, measured current is 
        used as is ,as average current is obtained */
    pfcParam.averageCurrent = PFC_SampleCorrection(boostDutyRatio,
                                    pfcPICurrent.output,pfcMeasured.current);

    /** PI Execution - PFC Current Control  */
    PFC_PIController(&pfcPICurrent,currentReference-pfcParam.averageCurrent);

    /** Calculate Duty Cycle of PWM Controlling PFC based on 
        as a ratio of PWM Period */
    pfcParam.duty  = (__builtin_mulss(pfcPICurrent.output, PFC_LOOPTIME_TCY) >> 15);

    /** Perform Boundary check of PWM Duty cycle */
    if (pfcParam.duty  > PFC_MAX_DUTY)
    {
        pfcParam.duty = PFC_MAX_DUTY;
        pfcPICurrent.integralOut = KI_I_INTGRAL_OUT_MAX;       
    }
    else if (pfcParam.duty  < PFC_MIN_DUTY)
    {
        pfcParam.duty  = PFC_MIN_DUTY;
    }
}
/**
 * Function to calculate current reference to track the input voltage
 * @param none
 * @return none
 * @example
 * <code>
 * PFC_CurrentRefGenerate();
 * </code>
 */
inline static void PFC_CurrentRefGenerate(void)
{
    int16_t currentRef1 =  0,currentRef2 = 0;
    
    /** PI loop controlling - PFC output voltage 
        Voltage PI is called rate specified by VOLTAGE_LOOP_EXE_RATE */
    if (pfcParam.voltLoopExeRate > VOLTAGE_LOOP_EXE_RATE)
    {
        pfcPIVoltage.error = pfcPIVoltage.reference-pfcVdcAVG.output;

        if((pfcPIVoltage.error > 700) || (pfcPIVoltage.error < -700 ))
        {
            pfcPIVoltage.ki = KI_V >> 1;
        }
        else
        {
            pfcPIVoltage.ki = KI_V;
        }
        PFC_PIController(&pfcPIVoltage,pfcPIVoltage.error);
        pfcParam.voltLoopExeRate = 0;
    }
    else
    {
       pfcParam.voltLoopExeRate++; 
    }
#ifdef PFC_POWER_CONTROL    
    /** Current reference Calculation is shown below
        Current reference =
                      (Voltage PI O/P)*(Measured Vac I/P )*(1/VacRMSqr)*KMUL
    */ 

    /** Step 1: Current reference calculation :  
            (Max Voltage PI O/P)*(Min Vac )

        Multiply Voltage PI output with Vac measured value and right shift 
        it by 18(= 15+3) to make sure result is always less than 
        VacRMS^2 .    

        Note that additional right shift by 3 is compensated in the 
        second step in the current reference calculation */
    currentRef1 = (int16_t) ((__builtin_mulss(pfcPIVoltage.output, 
                                            pfcMeasured.acVoltage)) >> 18);

    /** Step 2: Current reference calculation 
        ((Max Voltage PI O/P)*(Vac )*(1/VacRMS)^2))  
        That is divide first step value by  VacRMS^2 */
    if(pfcVacRMS.sqrOutput > 0)
    {
        currentRef2 = (int16_t)(__builtin_divf(currentRef1,pfcVacRMS.sqrOutput));
    }
    /** Step 3:  Current Reference Calculation 
        Multiply second step result with KMUL and right shift by 12 to 
        compensate for the right shift by 3 in the Step 1(above) */
    currentReference = (int16_t)((__builtin_mulss(currentRef2,KMUL)) >> 12);
        
#else
    refcur =     (__builtin_mulss(pfcPIVoltage.output, 
                                            pfcMeasured.acVoltage));
    if(pfcVacRMS.peak >0)
    {    
        currentReference = (int16_t)(__builtin_divsd(refcur,pfcVacRMS.peak));
    }
#endif        
    /**  Perform Boundary check of generated current reference */
    if (currentReference > Q15(0.999))
    {
        currentReference = Q15(0.999);
    }
    else if (currentReference < 0)
    {
        currentReference = 0;
    }
}
