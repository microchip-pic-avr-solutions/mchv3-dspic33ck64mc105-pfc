; ***********************************************************************
;                                                                      * 
;     Filename:       mc_invclarke_NoAccum.s                           *
;                                                                      *
; ***********************************************************************
; **********************************************************************
;
;
; (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use
; this software and any derivatives exclusively with Microchip products.
;
; This software and any accompanying information is for suggestion only.
; It does not modify Microchip's standard warranty for its products.
; You agree that you are solely responsible for testing the software and
; determining its suitability.  Microchip has no obligation to modify,
; test, certify, or support the software.

; THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES,
; WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
; INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
; AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH
; MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
; APPLICATION.

; IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL,
; PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF
; ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
; MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
; FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL
; LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
; EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO
; MICROCHIP FOR THIS SOFTWARE.

; MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF
; THESE TERMS.

          .include "mc_interfaces_dspic.inc"
; Inputs:
;  ALPHABETA.alpha
;  ALPHABETA.beta

; Outputs:
;  ABC.a
;  ABC.b
;  ABC.c		  
		  
; Constants
          .equ Sq3OV2u,  0xDDB4 ; sqrt(3)/2 in 0.16 format
	  .equ signBit,  15
; Note
;  This function requires CORCON register to be setup in a certain state
;  in order to operate correctly. Due to this requirement, this function
;  will save the CORCON register on the stack in the beginning of the
;  function and restore it before the function return.
;  After saving the CORCON register, this function writes to all bits of
;  the CORCON register. Thus, for the brief duration when this function is
;  executing, the state of CORCON register may be different from its state
;  as set by the function caller. This may change the CPU core behavior with
;  respect to exception processing latency, DO loop termination, CPU interrupt
;  priority level and DSP-engine behavior.

;=================== CODE =====================

          .section  .text
          .global   _MC_TransformClarkeInverseNoAccum_Assembly
          .global    MC_TransformClarkeInverseNoAccum_Assembly

_MC_TransformClarkeInverseNoAccum_Assembly:
MC_TransformClarkeInverseNoAccum_Assembly:
     ;; w0 : ALPHABETA.*
     ;; w1 : ABC.*
     ;; abc->a = alphabeta->alpha;
	mov       [w0+ALPHABETA_alpha], w3
	mov       w3, [w1+ABC_a]        
     ;;  x = -(alphabeta->alpha >> 1);
	asr       w3, w3
	neg       w3, w3
     ;;  y = (__builtin_mulus( MC_SQ3OV2U,alphabeta->beta )) >> 16;
	mov       [w0+ALPHABETA_beta], w4
	mov       #Sq3OV2u, w2
	mul.us    w2, w4, w4
     ;;  round off correction 
	btsc      w4,#signBit
	inc       w5, w5
     ;; abc->b = (-alpha/2) + (sqrt(3)/2)*beta  
	add       w5, w3, w0
	mov       w0, [w1+ABC_b]
     ;; abc->c = (-alpha/2) - (sqrt(3)/2)*beta  
	sub       w3, w5, w2
	mov       w2, [w1+ABC_c]	  

	return
.end
