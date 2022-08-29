;***********************************************************************
;                                                                      * 
;     Filename:       mc_park_dspic.s                                  *
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
;******************************************************************************/

          .include "mc_interfaces_dspic.inc"
; Inputs:
;  SINCOS.cos
;  SINCOS.sin
;  ALPHABETA.alpha
;  ALPHABETA.beta

; Outputs:
;  DQ.d
;  DQ.q

; Register usage
          .equ workW0,  w0    ; working register
	      .equ workW1,  w1    ; working register
	      .equ workW2,  w2    ; working register
          .equ CorconW4,w4    ; CORCON temp
          .equ sinW4,   w4    ; Sine
          .equ cosW5,   w5    ; Cosine
          .equ alphaW6, w6    ; alpha
          .equ betaW7,  w7    ; beta

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
          .global   _MC_TransformPark_Assembly
          .global   MC_TransformPark_Assembly

_MC_TransformPark_Assembly:
MC_TransformPark_Assembly:
     ;; Save CORCON, then configure as needed
          push      CORCON
          mov.w     #0x00E2,CorconW4
          mov.w     CorconW4,CORCON
     ;; workW0 : ALPHABETA*
	 ;; workW1 : SINCOS*
	 ;; workW2 : DQ*
          
     ;; Get sin, cos from SINCOS structure
          mov.w     [workW1+SINCOS_sin],sinW4
          mov.w     [workW1+SINCOS_cos],cosW5

     ;; Get alpha, beta from ALPHABETA structure
          mov.w     [workW0+ALPHABETA_alpha],alphaW6
          mov.w     [workW0+ALPHABETA_beta],betaW7		  
		  
     ;; d = alpha*cos(Angle) + beta*sin(Angle)
          mpy       sinW4*betaW7,A          ; beta*sin -> A
          mac       cosW5*alphaW6,A         ; add alpha*cos to A
          sac       A,workW0                ; store A into workW0
		  mov.w     workW0,[workW2+DQ_d]    ; Update d

     ;; q = -alpha*sin(Angle) + beta*cos(Angle)
          mpy       cosW5*betaW7,A          ; beta*cos -> A
          msc       sinW4*alphaW6,A         ; sub alpha*sin from A
          sac       A,workW0                ; store A into workW0
		  mov.w     workW0,[workW2+DQ_q]    ; Update q
		  
	 ;; Return 1	  
		  mov.w     #1, workW0		  
          pop       CORCON
          return

          .end
