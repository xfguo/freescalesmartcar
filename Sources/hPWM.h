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
  hPWM.h
  PWM 控制头文件
******************************************************************************/

extern void InitPWM01(void);
extern void SetPWM01(INT16U period, INT16U duty);
#define StartPWM01()	{PWME_PWME1 = 1;}
#define StopPWM01()		{PWME_PWME1 = 0;}

extern void InitPWM23(void);
extern void SetPWM23(INT16U period, INT16U duty);
#define StartPWM23()	{PWME_PWME3 = 1;}
#define StopPWM23()		{PWME_PWME3 = 0;}

extern void SetPWM45(INT16U period, INT16U duty);
extern void InitPWM45(void);
#define StartPWM45()	{PWME_PWME5 = 1;}
#define StopPWM45()		{PWME_PWME5 = 0;}
