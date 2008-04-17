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
 

// 霍尔测速 (Hall Velocity Measurement)
// 通过定时取脉冲计数器里的值,计算后的出当前速度
// 所采集的最近一次速度放在SpeedTimeH 和 SpeedTimeL中

#include "includes.h"

// 车速计数器,以两次得到脉冲间隔的毫秒数计
INT16U carSpeedCounter[4];
INT16U carSpeedArrN = 0;
unsigned long lastAbsoluteTime;
//INT16U last_TC2, last_tc2_counter;

extern INT32U PerDistSrvTotal;
extern INT16U PerDistSrvN;

#define SRV_LEFT 0
#define SRV_CENTER 1
#define SRV_RIGHT 2

// 相差多少
#define DIS(x,y) ( ( (x) > (y) ) ? ( (x) - (y) ) : ( (y) - (x) ))

INT8U LastSrvAvg = 90;

INT16U PerDistSrvAvg = 90;
INT16U LastSrvState = SRV_CENTER;


// 以轮转圈数来计量路程
//#define ClearDistanceCounter() {PAC = 0;}
//#define GetDistance() (PAC)
//#define Distance PAC

INT16U GetSpeed(void) {
    //如果还没开始跑,返回零
    if (GetDistance() < 8)
        return 0xFFFF;

    return  carSpeedCounter[0] +
            carSpeedCounter[1] +
            carSpeedCounter[2] +
            carSpeedCounter[3];
}

void ProcessSpeed_FUNC(void) {
    carSpeedCounter[carSpeedArrN = (carSpeedArrN + 1) & 0x3] = (INT16U)(absoluteTime - lastAbsoluteTime);
    lastAbsoluteTime = absoluteTime;

    /******************************************************/
    /** TODO 路径记忆的间隔分析 **/
    /* 分析前面的开始新的 */
    if (PerDistSrvTotal != 0 && PerDistSrvN != 0) {
        PerDistSrvAvg = (INT8U)(PerDistSrvTotal / PerDistSrvN);

        // 排除错误
        if (DIS(PerDistSrvAvg, LastSrvAvg) > 30) {
            PerDistSrvAvg = LastSrvAvg;
        }

        if ((PerDistSrvAvg > 50 && PerDistSrvAvg < 130) ) {
            LastSrvAvg = (INT8U)PerDistSrvAvg;
        }



    } else {
        PerDistSrvAvg = LastSrvAvg;
    }



    // 发送调试数据
    if (SCI0SR1 & 0x80) {
        SCI0DRL = (INT8U)((INT16S)((3927 * 4 * 4) / GetSpeed() * _RTI_P / 4));
        //(INT8U)PerDistSrvAvg;
    }

    /** 路径记忆入口点 **/
    Process(GetDistance(), (INT8U)PerDistSrvAvg);

    // 路径播放入口点
    if (PathMemState == PLAY) {
        PathPlayProcess(GetDistance());
        PathPlayer(GetDistance());
    }

    // 判断特殊点
    CheckNewPoint();

    PerDistSrvTotal = 0;
    PerDistSrvN = 0;
    /******************************************************/
}

// 初始化测速器
void InitSpeeder(void) {
    // 初始化和脉冲计数器
    // WaitDisable();
    WaitEnable();

    InitPA();
    PAI_FUNC = ProcessSpeed_FUNC;

    lastAbsoluteTime = 0;

    // 各位赋初值
    ClearDistanceCounter();

}





