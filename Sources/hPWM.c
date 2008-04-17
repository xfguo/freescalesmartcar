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
 

/******************************************************************************
  hPWM.c
  PWM 控制
******************************************************************************/
#include "includes.h"



/* 注意 *   当总线频率为8MHz时, 且PWMCLK_PCLK1 = 0x0,选用时钟A,
            PWMPRCLK_PCKA = 0x3,PWM一个周期的最小分度为一微妙
            所以1ms = 1000,当倍频到24MHz时1ms为 3000
*/
// ****************************************************************************
// 专有函数

// ----------------------------------------------------------------------------
// 控制PWM01口,16位
void SetPWM01(INT16U period, INT16U duty) {
    if ( duty < 3880 || duty > 5120) {

#if DEBUG == ON
        //HAVE_AN_ERROR();
#endif

        return;
    }
    PWMPER01 = period;
    PWMDTY01 = duty;
}

void InitPWM01(void) {
    PWMCTL_CON01 = 0x1;		/* 连接PWM0 和 1 */

    PWMPOL_PPOL1 = 0x1;		/* 开始是高 */
    PWMCLK_PCLK1 = 0x0;   	/* 选用时钟A, 一为SA */
    PWMPRCLK_PCKA = 0x3;  	/* 把时钟A分为原来的8分之1 */
    PWMCAE_CAE1 = 0x0;		/* 左对齐 */
    PWMCNT01 = 0x0;			/* 清零 PWM计数器,置多少都一样 */
    /*	PWMSCLA = ?;            设置SA把时钟A分多少频，当你用SA的时候 */
}

//#define StartPWM01()	{PWME_PWME1 = 1;}
//#define StopPWM01()		{PWME_PWME1 = 0;}


// ----------------------------------------------------------------------------
// 控制PWM23口,16位
void SetPWM23(INT16U period, INT16U duty) {
    if ( period < 100 ) {
        HAVE_AN_ERROR();
        return;
    }
    PWMPER23 = period;
    PWMDTY23 = duty;
}

void InitPWM23(void) {
    PWMCTL_CON23 = 0x1;		/* 连接PWM 2 和 3 */

    PWMPOL_PPOL3 = 0x0;	/* 开始是低 */
    PWMCLK_PCLK3 = 0x0;   /* 选用时钟B, 一为SB */
    PWMPRCLK_PCKB = 0x3;  /* 把时钟B分为原来的8分之1 */
    PWMCAE_CAE3 = 0x0;		/* 左对齐 */
    PWMCNT23 = 0x0;			/* 清零 PWM计数器,置多少都一样 */
    /*	PWMSCLA = ?;            设置SA把时钟A分多少频，当你用SA的时候 */
}

//#define StartPWM23()	{PWME_PWME3 = 1;}
//#define StopPWM23()		{PWME_PWME3 = 0;}

//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 控制PWM45口,16位
void SetPWM45(INT16U period, INT16U duty) {
    if ( period < 100 ) {
        HAVE_AN_ERROR();
        return;
    }
    PWMPER45 = period;
    PWMDTY45 = duty;
}

void InitPWM45(void) {
    PWMCTL_CON45 = 0x1;		/* 连接PWM 4 和 5 */

    PWMPOL_PPOL5 = 0x0;	/* 开始是低 */
    PWMCLK_PCLK5 = 0x0;   /* 选用时钟B, 一为SB */
    PWMPRCLK_PCKA = 0x3;  /* 把时钟B分为原来的8分之1 */
    PWMCAE_CAE5 = 0x0;		/* 左对齐 */
    PWMCNT45 = 0x0;			/* 清零 PWM计数器,置多少都一样 */
    /*	PWMSCLA = ?;            设置SA把时钟A分多少频，当你用SA的时候 */
}

//#define StartPWM45()	{PWME_PWME5 = 1;}
//#define StopPWM45()		{PWME_PWME5 = 0;}
// ****************************************************************************
// 通用函数


