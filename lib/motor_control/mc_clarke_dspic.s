;***********************************************************************
;                                                                      * 
;     Filename:       mc_clarke_dspic.s                                *
;                                                                      *
;***********************************************************************
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
;  ABC.a
;  ABC.b

; Outputs:
;  ALPHABETA.alpha
;  ALPHABETA.beta

; Register usage
          .equ workW0,  w0  ; working register
	  .equ workW1,  w1  ; working register
		  
          .equ Sq3W4,   w4  ; OneBySq3
          .equ CorconW4, w4 ; CORCON temp

          .equ aW6,    w6   ; a
          .equ bW7,    w7   ; b
          .equ betaW7, w7   ; beta replaces b
		  
; Constants
          .equ OneBySq3,0x49E7   ; 1/sqrt(3) in 1.15 format

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
          .global   _MC_TransformClarke_Assembly
          .global   MC_TransformClarke_Assembly

_MC_TransformClarke_Assembly:
MC_TransformClarke_Assembly:
     ;; Save CORCON, then configure as needed
          push      CORCON
          mov.w     #0x00E2,CorconW4
          mov.w     CorconW4,CORCON
     ;; workW0 : ABC*
     ;; workW1 : ALPHABETA*

          mov.w     #OneBySq3,Sq3W4     ; 1/sqrt(3) in 1.15 format

     ;; alpha = a
          mov.w     [workW0+ABC_a],aW6
          mov.w     aW6,[workW1+ALPHABETA_alpha]		  

     ;; beta = a*OneBySq3 + 2*b*OneBySq3
          mpy       Sq3W4*aW6,A
          mov.w     [workW0+ABC_b],bW7
          mac       Sq3W4*bW7,A
          mac       Sq3W4*bW7,A
          sac       A,betaW7
          mov.w     betaW7,[workW1+ALPHABETA_beta]
		  
	 ;; Return 1	  
		  mov.w     #1, workW0		  
          pop       CORCON
          return
          .end
