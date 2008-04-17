; ********* FuzzyEngine.asm ******************
; DC motor, Fuzzy Logic control
; Copyright 2004 by Jonathan W. Valvano, valvano@mail.utexas.edu 
;    You may use, edit, run or distribute this file 
;    as long as the above copyright notice remains 
; Last modified 5/20/2004 by Jonathan W. Valvano
 
;  This example accompanies the books
;   "Embedded Microcomputer Systems: Real Time Interfacing", Brooks-Cole, copyright (c) 2000,
;   "Introduction to Embedded Microcomputer Systems: 
;    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002
;
; modified for lab handout LM7  --  (fw-07-04)


DEFAULT_RAM:     section  

; -------------------------------------------------------------
; Fuzzy Membership sets
; -------------------------------------------------------------
; input membership variables
          absentry fuzvar 
fuzvar:   ds.b 8    ; inputs 
spErr_very_neg: equ  0    ; speed error very negative
spErr_negative: equ  1    ; speed error negative
spErr_zero:     equ  2    ; speed error zero
spErr_positive: equ  3    ; speed error positive
spErr_very_pos: equ  4    ; speed error very positive
accel_negative: equ  5    ; acceleration negative
accel_zero:     equ  6    ; acceleration zero
accel_positive: equ  7    ; acceleration positive

;output membership variables
          absentry fuzout 
fuzout:   ds.b 5    ; outputs   
power_bigsub:   equ  8    ; subtract some power from system
power_sub:      equ  9    ; subtract some power from system
power_zero:     equ  10   ; leave power as is
power_add:      equ  11   ; add some power to system
power_bigadd:   equ  12   ; add a lot of power to system
; -------------------------------------------------------------


NON_BANKED:       section    

; -------------------------------------------------------------
; fuzzification
; -------------------------------------------------------------
; to be completed ...
s_tab:    dc.b  0, 85, 0, 13    ; spErr_very_neg (0,0), (85,0), inf, 255/(85-65)
          dc.b  42, 128, 4, 10  ; spErr_negative (42,0), (128,0), 255/(103-42), 255/(128-103)
          dc.b  85, 170, 6, 6   ; spErr_zero		  (85,0), (170,0), 6, 6 
          dc.b  128, 213, 10, 4 ; spErr_positive (128,0), (213,0), 255/(154-128), 255/(213-154)
          dc.b  170, 255, 13, 0 ; spErr_very_pos (170,0), (255,0), 255/(194-170), inf
a_tab:    dc.b  0, 128,0 ,8     ; accel_negative (0,0), (128,0), inf, 255/(128-64) = 8
          dc.b  64, 191, 8, 8   ; accel_zero     (64,0), (191,0), 8, 8
          dc.b  128, 255, 8, 0  ; accel_positive (128,0), (255,0), 8, inf
; ... to be completed 
; -------------------------------------------------------------


; -------------------------------------------------------------
; rule base
; -------------------------------------------------------------
rules:
; to be completed ...
          dc.b  spErr_very_neg, accel_negative, $FE, power_add,  $FE
          dc.b  spErr_very_neg, accel_zero,     $FE, power_add,  $FE
          dc.b  spErr_very_neg, accel_positive, $FE, power_add,  $FE
          dc.b  spErr_negative, accel_negative, $FE, power_add,  $FE
          dc.b  spErr_negative, accel_zero,     $FE, power_zero, $FE
          dc.b  spErr_negative, accel_positive, $FE, power_zero, $FE
          dc.b  spErr_zero,     accel_negative, $FE, power_zero, $FE
          dc.b  spErr_zero,     accel_zero,     $FE, power_zero, $FE
          dc.b  spErr_zero,     accel_positive, $FE, power_zero, $FE
          dc.b  spErr_positive, accel_negative, $FE, power_zero, $FE
          dc.b  spErr_positive, accel_zero,     $FE, power_zero, $FE
          dc.b  spErr_positive, accel_positive, $FE, power_sub,  $FE
          dc.b  spErr_very_pos, accel_negative, $FE, power_sub,  $FE
          dc.b  spErr_very_pos, accel_zero,     $FE, power_sub,  $FE
          dc.b  spErr_very_pos, accel_positive, $FE, power_sub,  $FE
          dc.b  $FF  
; ... to be completed
; -------------------------------------------------------------


; -------------------------------------------------------------
; defuzzification
; -------------------------------------------------------------
output_singletons:
     ; MoM -- Mean of Maximum (using 0 ... 255 range)
     ; power_bigsub, power_sub, power_zero, power_add, power_bigadd
     dc.b (64-0)/2, 20, 128, 230, (255+190)/2
; -------------------------------------------------------------



; -------------------------------------------------------------
; FuzzyLogic
;
; RegB=SpeedError, 0 to 255, 128 means no error
; Stack parameter 5,sp is 16-bit Acceleration, 0 to 255
; Stack parameter 6,sp is 8-bit Acceleration, 128 means no acceleration
; return dU, -128 to +127, 0 means no change    
; -------------------------------------------------------------
          absentry FuzzyLogic 
FuzzyLogic:  
    pshx
    pshb      
    ;ldaa 6,sp      ; RegA=SpeedError, 0 to 255
    ldx  #s_tab
    ldy  #fuzvar
    mem
    mem
    mem
    mem
    mem

    pula    ;RegA=Acceleration, 0 to 255
    ldx  #a_tab
    mem
    mem
    mem

    ldab #5     ; number of output fuzzy membership sets
cloop:
    clr  1,y+   ; clear output fuzzy variables
    dbne b,cloop
    ldx  #rules
    ldy  #fuzvar
    ldaa #$FF
    rev
    ldy  #fuzout
    ldx  #output_singletons
    ldab #5
    wav
    ediv         ; Y has weighted average, 0 to 255
    leay -128,y  ; adjust for offset, -128 to +127
    tfr  y,d     ; return dpower
    pulx
    rtc
    