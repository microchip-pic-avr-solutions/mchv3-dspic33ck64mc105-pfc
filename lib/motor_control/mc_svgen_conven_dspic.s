;***********************************************************************
;                                                                      * 
;     Filename:       mc_svgen_conven_dspic.s                          *
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
;  ABC.c
;  iPwmPeriod

; Outputs:
;  DUTYCYCLEOUT.dutycycle1
;  DUTYCYCLEOUT.dutycycle2
;  DUTYCYCLEOUT.dutycycle3

; Register usage
          .equ workW0,      w0  ; Working register             
          .equ workW1,      w1  ; Working register
          .equ workW2,      w2  ; working register
          .equ workW4,      w4  ; working register	  
          .equ workW5,      w5  ; working register
          .equ workW6,      w6  ; working register
	  .equ t1W2,        w2
          .equ workDLoW2,   w2  ; double word (multiply results)	  
          .equ workDLoW4,   w4  ; double word (multiply results)
	  .equ workDLoW6,   w6  ; double word (multiply results)
          .equ aW2,         w2
          .equ taW4,        w4
          .equ workDHiW5,   w5  ; double word (multiply results)
	  .equ workDHiW7,   w7  ; double word (multiply results)
          .equ bW3,         w3
          .equ tbW5,        w5
          .equ cW4,         w4
          .equ tcW6,        w6
	  .equ BaseptrW10,  w10
; Constants
          .equ TwoOVsq3u,   0x93cd ; sqrt(3)/2 in 0.16 format
		  
;=================== CODE =====================

          .section  .text
          .global   _MC_CalculateSpaceVector_Assembly
          .global   MC_CalculateSpaceVector_Assembly

_MC_CalculateSpaceVector_Assembly:
MC_CalculateSpaceVector_Assembly:
     ;; workW0 : ABC*
     ;; workW1 : iPwmPeriod
     ;; workW2 : DUTYCYCLEOUT*
          push       BaseptrW10
     ;; Move W2 to W10 to make way for multiplications
          mov.w     workW2,BaseptrW10
     ;; Multiply inputs with 2* 1/sqrt(3)
     ;; 2 for easy divide by s^16 and 1/sqrt(3) for 
     ;; Compensation for gain of sqrt(3) on subtraction
          mov       #TwoOVsq3u, t1W2     
 	  mul.us    t1W2,[workW0++],workDLoW6   
          mul.us    t1W2,[workW0++],workDLoW4
          mul.us    t1W2,[workW0],workDLoW2
     ;; Compute differences to add 30 degree offset
     ;; and gain of sqrt(3)
	  sub       w5,w3,w2   ; a
	  sub       w3,w7,w4   ; c
	  sub       w7,w5,w3   ; b
	   
     ;; Test a
           cp0       aW2
           bra       LT,jCalcRef20       ; aW2 < 0

     ;; Test b
           cp0       bW3
           bra       LT,jCalcRef10       ; bW3 < 0

     ;; Must be Sector 3 since Sector 7 not allowed
     ;; Sector 3: (0,1,1)  0-60 degrees

     ;; T1 = a
     ;; T2 = b
     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0
		  
     ;; T1 = PWM*T1		  
          mul.us    workW0,aW2,workDLoW4

     ;; T2 = PWM*T2
          mul.us    workW0,bW3,workDLoW6

     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4

     ;; dutycycle1 = Ta
     ;; dutycycle2 = Tb
     ;; dutycycle3 = Tc
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;;	  return    1
          mov.w     #1, workW0
          pop       BaseptrW10          
          return


jCalcRef10:
     ;; Test c
          cp0       cW4
          bra       LT,jCalcRef15       ; cW4 < 0

     ;; Sector 5: (1,0,1)  120-180 degrees
     ;; T1 = c
     ;; T2 = a

     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0

     ;; T1 = PWM*T1
          mul.us    workW0,cW4,workDLoW4

     ;; T2 = PWM*T2
          mul.us    workW0,aW2,workDLoW6
     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4


     ;; dutycycle1 = Tc
     ;; dutycycle2 = Ta
     ;; dutycycle3 = Tb
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;; return 1
          mov.w     #1, workW0
          pop       BaseptrW10          
          return


jCalcRef15:

     ;; Sector 1: (0,0,1)  60-120 degrees
     ;; T1 = -c
     ;; T2 = -b
          neg.w     bW3,bW3
          neg.w     cW4,cW4

     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0

     ;; T1 = PWM*T1
          mul.us    workW0,cW4,workDLoW4

     ;; T2 = PWM*T2
          mul.us    workW0,bW3,workDLoW6

     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4

     ;; dutycycle1 = Tb
     ;; dutycycle2 = Ta
     ;; dutycycle3 = Tc
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;;	  return 1
          mov.w     #1, workW0
          pop       BaseptrW10          
          return


jCalcRef20:

     ;; Test b
          cp0       bW3
          bra       LT,jCalcRef30       ; bW3 < 0

     ;; Test c
          cp0       cW4
          bra       LT,jCalcRef25       ; cW4 < 0

     ;; Sector 6: (1,1,0)  240-300 degrees
     ;; T1 = b
     ;; T2 = c

     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0

     ;; T2 = PWM*T2
          mul.us    workW0,cW4,workDLoW6
	  
     ;; T1 = PWM*T1
          mul.us    workW0,bW3,workDLoW4

     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4


     ;; dutycycle1 = Tb
     ;; dutycycle2 = Tc
     ;; dutycycle3 = Ta
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;; Return 1	  
	  mov.w     #1, workW0
          pop       BaseptrW10          
          return


jCalcRef25:
     ;; Sector 2: (0,1,0)  300-360 degrees
     ;; T1 = -a
     ;; T2 = -c
          neg.w     cW4,cW4
          neg.w     aW2,aW2

     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0

	  
     ;; T2 = PWM*T2
          mul.us    workW0,cW4,workDLoW6

     ;; T1 = PWM*T1
          mul.us    workW0,aW2,workDLoW4

     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4

     ;; dutycycle1 = Ta
     ;; dutycycle2 = Tc
     ;; dutycycle3 = Tb
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;; Return 1	  
	  mov.w     #1, workW0
          pop       BaseptrW10          
          return

jCalcRef30:
     ;; Must be Sector 4 since Sector 0 not allowed
     ;; Sector 4: (1,0,0)  180-240 degrees

     ;; T1 = -b
     ;; T2 = -a
          neg.w     aW2,aW2
          neg.w     bW3,bW3

     ;; Since T1 is in 1.15 and PWM in integer we do multiply by
     ;; PWM*T1 as integers and use upper word of results

     ;;  PWMPeriod x 2 for easy divide by 2^16
	  sl.w      workW1,workW0

     ;; T1 = PWM*T1
          mul.us    workW0,bW3,workDLoW4

     ;; T2 = PWM*T2
          mul.us    workW0,aW2,workDLoW6
     ;; Tc = (PWM-T1-T2)/2
          sub.w     workW1,workDHiW5,workW1     ;PWM-T1
          sub.w     workW1,workDHiW7,workW1     ; -T2
          asr.w     workW1,workW6		; /2

     ;; Tb = Tc + T1
          add       workW6,workDHiW5,workW5

     ;; Ta = Tb + T2
          add       workW5,workDHiW7,workW4

     ;; dutycycle1 = Tc
     ;; dutycycle2 = Tb
     ;; dutycycle3 = Ta
          mov.w     tcW6,[BaseptrW10+DUTYCYCLEOUT_dutycycle1]
          mov.w     tbW5,[BaseptrW10+DUTYCYCLEOUT_dutycycle2]
          mov.w     taW4,[BaseptrW10+DUTYCYCLEOUT_dutycycle3]
     ;; Return 1	  
	  mov.w     #1, workW0
          pop       BaseptrW10          
          return




