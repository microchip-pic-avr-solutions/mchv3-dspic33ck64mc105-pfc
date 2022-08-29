; ***********************************************************************
;                                                                      * 
;     Filename:       mc_invpark_dspic.s                               *
;                                                                      *
; ***********************************************************************
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
;  DQ.d
;  DQ.q
;  SINCOS.cos
;  SINCOS.sin

; Outputs:
;  ALPHABETA.alpha
;  ALPHABETA.beta

; Register usage
          .equ workW0,  w0  ; working register
	      .equ workW1,  w1  ; working register
	      .equ workW2,  w2  ; working register          
          .equ workW3,  w3  ; working register
          .equ CorconW4,w4  ; CORCON temp
          .equ sinW4,   w4  ; sin
          .equ cosW5,   w5  ; cos
          .equ dW6,     w6  ; d
          .equ qW7,     w7  ; q

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
          .global   _MC_TransformParkInverse_Assembly
          .global   MC_TransformParkInverse_Assembly

_MC_TransformParkInverse_Assembly:
MC_TransformParkInverse_Assembly:
     ;; Save CORCON, then configure as needed
          push      CORCON
          mov.w     #0x00E2,CorconW4
          mov.w     CorconW4,CORCON
     ;; workW0 : DQ*
	 ;; workW1 : SINCOS*
	 ;; workW2 : ALPHABETA*
	 
     ;; Get d, q from DQ structure
          mov.w     [workW0+DQ_d],dW6
          mov.w     [workW0+DQ_q],qW7

     ;; Get cos and sine from SINCOS structure
          mov.w     [workW1+SINCOS_cos],cosW5
          mov.w     [workW1+SINCOS_sin],sinW4
          
     ;; alpha = d*cos(Angle) - q*sin(Angle)
          mpy       cosW5*dW6,A     ; Cos*d -> A
          msc       sinW4*qW7,A     ; sub Sin*q from A
          sac       A,workW3        ; store to alpha
		  mov.w		workW3,[workW2+ALPHABETA_alpha]

     ;; beta = d*sin(Angle) + q*cos(Angle)
          mpy       sinW4*dW6,A     ; Sin*d -> A
          mac       cosW5*qW7,A     ; add Cos*q to A
          sac       A,workW3        ; store to beta
		  mov.w		workW3,[workW2+ALPHABETA_beta]

	 ;; Return 1	  
		  mov.w     #1, workW0
          pop       CORCON
          return
          .end
