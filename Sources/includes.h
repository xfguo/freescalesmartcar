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


#pragma LINK_INFO DERIVATIVE "mc9s12dg128b"
/******************************************************************************
  includes.h
  SmartCar 项目主头文件
******************************************************************************/

#include <hidef.h>
#include <mc9s12dg128.h>

/******************************************************************************
  优化和调试选项
******************************************************************************/

#include "optimize.h"
#include "debug.h"

/******************************************************************************
  数据类型定义
******************************************************************************/
typedef unsigned char   INT8U;
typedef signed char     INT8S;
typedef unsigned short  INT16U;
typedef signed short    INT16S;
typedef unsigned long   INT32U;
typedef signed long     INT32S;

#define UB  INT8U
#define SB  INT8S
#define UW  INT16U
#define SW  INT16S

#define ISR __interrupt void

/******************************************************************************
  常用常量
*******************************************************************************/
#define FALSE   0
#define TRUE    1

#define ON 1
#define OFF 0

#define _RTI_P 16

/*******************************************************************************
  各模块头文件
*******************************************************************************/
#include "pll.h"

#include "public.h"

#include "hRTI.h"
#include "hPWM.h"
#include "hIO.h"
#include "hPA.h"
#include "hTimer.h"
#include "hAD.h"

#include "dHallVM.h"
#include "dServo.h"
#include "dMotor.h"
#include "dSmallButton.h"

#include "cMotorControl.h"
#include "cServoControl.h"
#include "cCoreControl.h"

#include "sPathMem.h"

#if DEBUG == ON

#include "dbgButton.h"
#include "dbgLCD.h"
#include "dbgSCI.h"
#include "dbgMenu.h"

#include "Test_All.h"

#endif

/*******************************************************************************
  常用宏
*******************************************************************************/
#define FOREVER()		for(;;)

// 字节取反
#define REV_BYTE(x)		{x = ~ (x);}
// abs
#define _abs(x) (((x) > 0)?(x):(-x))

#define HAVE_AN_ERROR()    {DDRB = 0xFF;PORTB = 0xAA;FOREVER();}

/******************************************************************************
 关于可变参数的一些定义
 *****************************************************************************/
typedef char *va_list;

#define _INTSIZEOF(n) ((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) )

#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )

#define va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )

#define va_end(ap) ( ap = (va_list)0 )

void ChgSet1(void);
void ChgSet2(void);
void ChgSet3(void);

extern INT16U maxSpeed;
extern INT16U minSpeed;

/******************************************************************************
 根据最大速度计算合适的减速距离
 ******************************************************************************/

extern INT16U MaxStraightSpdDwnLength;
extern INT16U MinStraightSpdDwnLength;

extern INT16U ShortestHighSpeedLength;

extern INT16U MSpeed;

extern INT16U StrightErrorLength;
