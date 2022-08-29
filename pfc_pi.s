;*******************************************************************************
; Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
;
; SOFTWARE LICENSE AGREEMENT:
;
; Microchip Technology Incorporated ("Microchip") retains all ownership and
; intellectual property rights in the code accompanying this message and in all
; derivatives hereto.  You may use this code, and any derivatives created by
; any person or entity by or on your behalf, exclusively with Microchip's
; proprietary products.  Your acceptance and/or use of this code constitutes
; agreement to the terms and conditions of this notice.
;
; CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
; WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
; TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
; PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
; PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
; YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
; WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
; STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
; FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
; LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
; HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
; THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
; MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
; SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
; HAVE THIS CODE DEVELOPED.
;
; You agree that you are solely responsible for testing the code and
; determining its suitability.  Microchip has no obligation to modify, test,
; certify, or support the code.
;
;******************************************************************************/

.ifdecl __ASM30__
    .include "xc.inc"
.endif

; Definitions	
.equ _output,0
.equ _integralOut,2
.equ _propOut,4

.equ _input,6
.equ _reference,8
.equ _error,10

.equ _kp,12
.equ _ki,14
.equ _kpScale,16
.equ _kiScale,18
.equ _outMin,20
.equ _outMax,22
        
.global   _PFC_PIController
.global   PFC_PIController

_PFC_PIController:
PFC_PIController:

    ; Save CORCON, then configure as needed
    push    CORCON
    mov     #0x00E0,w3
    mov     w3,CORCON
    
    ; Load error (Reference - measued) to w5 register
    mov     w1,[w0+_error]
    mov     w1,w5
    
    ; Load previous integral Output to Accumulator B 
    mov	    [w0+_integralOut],w3
    lac	    w3,b
    
    ; Load ki into W4 register
    mov	    [w0+_ki],w4
    ; Multiply ki * error and load into accumulator A
    ; A = ki * error 
    mpy	    w4*w5,a
    
    ; Scale accumualtor by same factor by which coeffiecients are scaled
    ; A = A * kiScale;
    mov    [w0+_kiScale],w3
    neg	    w3,w3
    sftac   a,w3
    
    ; Add previous intergral Output and Ki * error and store it in accumulator A
    ; A = integralOut + ki * error ; 
    add	    a
    
    ; Round accumulator A and move it to intergral output (integralOut)
    sac.r   a,w3
    mov	    w3,[w0+_integralOut]
    
   ; Load w4 with kp 
    mov	    [w0+_kp],w4
    
   ; Caluclate kp*error and load it in Accumulator B
   ; B = kp * error
    mpy	    w4*w5,b
    
    ; Scale accumualtor by same factor by which coeffiecients are scaled
    ; B = B * kpScale
    mov     [w0+_kpScale],w3
    neg	    w3,w3
    sftac   b,w3
    
    ; Round the accumulator B and load into propotional Output (propOut)
    sac.r   b,w3
    mov	    w3,[w0+_propOut]
 
    ; Add accumulator A and B
    ; B = kp * error + ki * error + integralOut(previous) and 
    add	    b
    sac.r   b,w3
    
    ; Bound check the PI output and limit it ouput between minimum and 
    ; maximum value specified.
    mov     [w0+_outMin], w6
    cpsgt   w3, w6
    mov     w6, w3
    mov     [w0+_outMax], w6
    cpslt   w3, w6
    mov     w6, w3
    mov	    w3,[w0+_output]
         
    pop CORCON
    return    
.end
