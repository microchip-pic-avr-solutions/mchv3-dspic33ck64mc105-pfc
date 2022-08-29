/*******************************************************************************
  Motor Control Library Interface Header File

  File Name:
    motor_control_mapping.h

  Summary:
    Library function name mapping file.

  Description:
    This header file defines a simple function name for each of the library 
    function and maps it, by default, to the prototype of one of the implementation 
    variants.
*******************************************************************************/
// DOM-IGNORE-BEGIN
/* *********************************************************************
 *
 * (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use
 * this software and any derivatives exclusively with Microchip products.
 *
 * This software and any accompanying information is for suggestion only.
 * It does not modify Microchip's standard warranty for its products.
 * You agree that you are solely responsible for testing the software and
 * determining its suitability.  Microchip has no obligation to modify,
 * test, certify, or support the software.
 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH
 * MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
 * APPLICATION.
 
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
 * PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF
 * ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
 * MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
 * FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
 * EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO
 * MICROCHIP FOR THIS SOFTWARE.
 
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
 * THESE TERMS.
 *
 * *****************************************************************************/

#ifndef _MOTOR_CONTROL_MAPPING_H_    // Guards against multiple inclusion
#define _MOTOR_CONTROL_MAPPING_H_


// *****************************************************************************

#ifdef MC_CONFIG_CALCULATESINECOSINE_ASSEMBLY_RAM
#define MC_CalculateSineCosine MC_CalculateSineCosine_Assembly_Ram
#elif defined(MC_CONFIG_CALCULATESINECOSINE_ASSEMBLY_FLASH)
#define MC_CalculateSineCosine MC_CalculateSineCosine_Assembly_Flash
#elif defined(MC_CONFIG_CALCULATESINECOSINE_INLINEC_RAM)
#define MC_CalculateSineCosine MC_CalculateSineCosine_InlineC_Ram
#elif defined(MC_CONFIG_CALCULATESINECOSINE_INLINEC_FLASH)
#define MC_CalculateSineCosine MC_CalculateSineCosine_InlineC_Flash
#else
#define MC_CalculateSineCosine MC_CalculateSineCosine_Assembly_Ram
#endif

// *****************************************************************************

#ifdef MC_CONFIG_TRANSFORMPARKINVERSE_ASSEMBLY
#define MC_TransformParkInverse MC_TransformParkInverse_Assembly
#elif defined(MC_CONFIG_TRANSFORMPARKINVERSE_INLINEC)
#define MC_TransformParkInverse MC_TransformParkInverse_InlineC
#else
#define MC_TransformParkInverse MC_TransformParkInverse_Assembly
#endif

// *****************************************************************************

#ifdef MC_CONFIG_TRANSFORMCLARKEINVERSELEGACY_ASSEMBLY
#define MC_TransformClarkeInverse MC_TransformClarkeInverseSwappedInput_Assembly
#elif defined(MC_CONFIG_TRANSFORMCLARKEINVERSELEGACY_INLINEC)
#define MC_TransformClarkeInverse MC_TransformClarkeInverseSwappedInput_InlineC
#else
#define MC_TransformClarkeInverse MC_TransformClarkeInverseSwappedInput_Assembly
#endif

// *****************************************************************************

#ifdef MC_CONFIG_CALCULATESPACEVECTORLEGACY_ASSEMBLY
#define MC_CalculateSpaceVector MC_CalculateSpaceVectorPhaseShifted_Assembly
#elif defined(MC_CONFIG_CALCULATESPACEVECTORLEGACY_INLINEC)
#define MC_CalculateSpaceVector MC_CalculateSpaceVectorPhaseShifted_InlineC
#else
#define MC_CalculateSpaceVector MC_CalculateSpaceVectorPhaseShifted_Assembly
#endif

// *****************************************************************************

#ifdef MC_CONFIG_TRANSFORMCLARKE_ASSEMBLY
#define MC_TransformClarke MC_TransformClarke_Assembly
#elif defined(MC_CONFIG_TRANSFORMCLARKE_INLINEC)
#define MC_TransformClarke MC_TransformClarke_InlineC
#else
#define MC_TransformClarke MC_TransformClarke_Assembly
#endif

// *****************************************************************************

#ifdef MC_CONFIG_TRANSFORMPARK_ASSEMBLY
#define MC_TransformPark MC_TransformPark_Assembly
#elif defined(MC_CONFIG_TRANSFORMPARK_INLINEC)
#define MC_TransformPark MC_TransformPark_InlineC
#else
#define MC_TransformPark MC_TransformPark_Assembly
#endif

// *****************************************************************************

#ifdef MC_CONFIG_CONTROLLERPIUPDATE_ASSEMBLY
#define MC_ControllerPIUpdate MC_ControllerPIUpdate_Assembly
#elif defined(MC_CONFIG_CONTROLLERPIUPDATE_INLINEC)
#define MC_ControllerPIUpdate MC_ControllerPIUpdate_InlineC
#else
#define MC_ControllerPIUpdate MC_ControllerPIUpdate_Assembly
#endif

// *****************************************************************************


#endif // _MOTOR_CONTROL_MAPPING_H_

// DOM-IGNORE-END
