;***********************************************************************
;                                                                      *
;     Filename:       mc_sinetable_ram_dspic.s                         *
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

;*************************************************************************

         .include "mc_interfaces_dspic.inc"

;=================== LOCAL DATA =====================

          .section .sinetbl,data
          .align	256          
          .global MC_SineTableInRam
          .global _MC_SineTableInRam
_MC_SineTableInRam:
MC_SineTableInRam:
  .word     0,  1608,  3212,  4808,  6393,  7962,  9512, 11039
  .word 12539, 14010, 15446, 16846, 18204, 19519, 20787, 22005
  .word 23170, 24279, 25329, 26319, 27245, 28105, 28898, 29621
  .word 30273, 30852, 31356, 31785, 32137, 32412, 32609, 32728
  .word 32767, 32728, 32609, 32412, 32137, 31785, 31356, 30852
  .word 30273, 29621, 28898, 28105, 27245, 26319, 25329, 24279
  .word 23170, 22005, 20787, 19519, 18204, 16846, 15446, 14010
  .word 12539, 11039,  9512,  7962,  6393,  4808,  3212,  1608
  .word     0, -1608, -3212, -4808, -6393, -7962, -9512,-11039
  .word-12539,-14010,-15446,-16846,-18204,-19519,-20787,-22005
  .word-23170,-24279,-25329,-26319,-27245,-28105,-28898,-29621
  .word-30273,-30852,-31356,-31785,-32137,-32412,-32609,-32728
  .word-32767,-32728,-32609,-32412,-32137,-31785,-31356,-30852
  .word-30273,-29621,-28898,-28105,-27245,-26319,-25329,-24279
  .word-23170,-22005,-20787,-19519,-18204,-16846,-15446,-14010
  .word-12539,-11039, -9512, -7962, -6393, -4808, -3212, -1608
  .word     0,  1608,  3212,  4808,  6393,  7962,  9512, 11039
  .word 12539, 14010, 15446, 16846, 18204, 19519, 20787, 22005
  .word 23170, 24279, 25329, 26319, 27245, 28105, 28898, 29621
  .word 30273, 30852, 31356, 31785, 32137, 32412, 32609, 32728
  .word 32767


               .end
  