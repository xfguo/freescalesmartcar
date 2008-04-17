/*
 * Copyright (c) 1998, Xiongfei Guo, Shanghai Second Polytechnic University 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Shanghai Second Polytechnic University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 

// Timer Input Capture/Output Compare
#include "includes.h"

// ----------------------------------------------------------------------------
// 定时器模块1 用作输出比较 产生一定周期的中断事件
INT16U tc1_counter;

// 当时间到的时候所执行的函数指针
void (* TC1OC_FUNC)(void) = NULL;

#pragma CODE_SEG __NEAR_SEG NON_BANKED
ISR TC1_ISR(void) {
    // 3 * 53333 产生一个 约20ms的中断
    if (++tc1_counter == 9) {
        // --------------
        // Do some thing
        if (TC1OC_FUNC != NULL) {
            TC1OC_FUNC();
        }
        // --------------
        tc1_counter = 0;
    }

    // 清除标志
    TFLG1_C1F = 1;

    TC1 -= 12202;//相当于:  TC1 += 53333;
}

#pragma CODE_SEG DEFAULT
void StartTC1(void) {
    TFLG1_C1F = 0;//清除标志
    TIE_C1I = 1;//使能中断
}

void StopTC1(void) {
    TFLG1_C1F = 1;//清除标志
    TIE_C1I = 0;//禁用中断
}

void InitTC1(void(*far tc1oc_func)) {
    EnableInterrupts;

    tc1_counter = 0;

    TC1OC_FUNC = tc1oc_func;

    TIOS_IOS1 = 1;// 设为输出比较
    TCTL2_OL0 = 0;// 使引脚与OCx断开,下同
    TCTL2_OM0 = 0;
    TFLG1_C1F = 1;// 先置标,StartTCx后在开始
    TSCR1_TEN = 1;// 使能计时器
}

// ----------------------------------------------------------------------------
// 定时器模块2 用作输出比较 产生一定周期的中断事件
INT16U tc2_counter;

#pragma CODE_SEG __NEAR_SEG NON_BANKED
ISR TC2_ISR(void) {
    tc2_counter++;

    // 清除标志
    TFLG1_C2F = 1;

    // 约3.556ms
    TC2--;
}

#pragma CODE_SEG DEFAULT


void StartTC2(void) {
    TFLG1_C2F = 0;//清除标志
    TIE_C2I = 1;//使能中断
}

void StopTC2(void) {
    TFLG1_C2F = 1;//清除标志
    TIE_C2I = 0;//禁用中断
}

void InitTC2(void) {
    EnableInterrupts;

    tc2_counter = 0;

    TIOS_IOS2 = 1;// 设为输出比较
    TCTL2_OL0 = 0;// 使引脚与OCx断开,下同
    TCTL2_OM0 = 0;
    TFLG1_C2F = 1;// 先置标,StartTCx后在开始
    TSCR1_TEN = 1;// 使能计时器
}

