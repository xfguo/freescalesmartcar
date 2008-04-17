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
 
// AD 转换模块

#include "includes.h"

// 在芯片中有两套同样地8路10位AD模块,共16路

// 初始化 AD 转换模块0 --------------------------------------------------------
// PS:必须初始化,再取值否则将陷入死循环

// 宏,初始化两个AD模块
//#define InitADC() {InitADC0();InitADC1();}

void InitADC0(void) {
    ATD0CTL2 = 0x80; // enable ADC
    // bit 7 ADPU=1 设置功率提升
    // bit 6 AFFC=0 ATD Fast Flag Clear All
    // bit 5 AWAI=0 ATD Power Down in Wait Mode
    // bit 4 ETRIGLE=0 External Trigger Level/Edge Control
    // bit 3 ETRIGP=0 External Trigger Polarity
    // bit 2 ETRIGE=0 External Trigger Mode Enable
    // bit 1 ASCIE=0 ATD Sequence Complete Interrupt Enable
    // bit 0 ASCIF=0 ATD Sequence Complete Interrupt Flag

    ATD0CTL4 = 0x05; // configure conversion
    // bit 7 SRES8=0 A/D Resolution Select
    //             1 = 8 bit resolution(8位AD)
    //             0 = 10 bit resolution(10位AD)
    // bit 6 SMP1=0 Sample Time Select
    // bit 5 SMP0=0 2 clock period
    // bit 4 PRS4=0 ATD Clock Prescaler divide by 12
    // bit 3 PRS3=0 ATD Clock Prescaler
    // bit 2 PRS2=1 ATD Clock Prescaler
    // bit 1 PRS1=0 ATD Clock Prescaler
    // bit 0 PRS0=1 ATD Clock Prescaler
}

// 初始化 AD 转换模块1, 基本同上
void InitADC1(void) {
    ATD1CTL2 = 0x80;
    ATD1CTL4 = 0x05;
}


// 读取某一通道AD数据 ---------------------------------------------------------

// 宏,从PAD0/AN0 ~ PAD15/AN15 中读取数值, channel -> 0 ~ 15
//#define ReadADC(channel) ((channel)>7) ? ReadADC1( (channel) - 8 ) : ReadADC0( (channel) )

// 从第一套AD中读值,PAD0/AN0 ~ PAD7/AN7
// 输入:channel -> 通道0 <= channel <= 7
INT16U ReadADC0(INT16U channel) {
    ATD0CTL5 = 	ATD0CTL5_DJM_MASK +  //数据右对齐
                (ATD0CTL5_Cx_MASK & channel); // 防止出错
    // 等待CCF0 ,即转换完成, 经计算此转换要在使 while 循环31次
    while ((ATD0STAT1&0x01)==0) {};
    return ATD0DR0;
}

// 从第二套AD中读值,PAD8/AN8 ~ PAD15/AN15
// 输入:channel -> 通道0 <= channel <= 7
INT16U ReadADC1(INT16U channel) {
    ATD1CTL5 = 	ATD1CTL5_DJM_MASK +
                (ATD1CTL5_Cx_MASK & channel);
    while ((ATD1STAT1&0x01)==0) {};
    return ATD1DR0;
}

// 同时读取两套AD的数据 -------------------------------------------------------

void ReadADCDouble(INT8U channel1, INT8U channel2, INT16U * a, INT16U * b) {

    channel2 -= 8;
#if OPTIMIZE == ON

    ATD0CTL5 = 	ATD0CTL5_DJM_MASK + channel1;
    ATD1CTL5 = 	ATD1CTL5_DJM_MASK + channel2;

#else

    ATD0CTL5 = 	ATD0CTL5_DJM_MASK +
                (ATD0CTL5_Cx_MASK & channel1);
    ATD1CTL5 = 	ATD1CTL5_DJM_MASK +
                (ATD1CTL5_Cx_MASK & channel2);
#endif

    while (!(ATD0STAT1_CCF0 || ATD1STAT1_CCF0)) {};

    *a = ATD0DR0;
    *b = ATD1DR0;
}
