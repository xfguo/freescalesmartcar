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

// Motor Control

#include "includes.h"

INT16U mduModify = 2;
INT16S pK = 15;
INT16S tmpSpeed;
INT16S mU, mdU;
INT16S mError, mAcc, last_mError, last2_mError;
INT16S last_v,v;
long motorTime, lastMotorTime;

INT16S md_Correct = 1;

// 刹车级别
INT16U breakLevel = 5;

INT8U readySend[7];
INT8U readySendN = 0;

void MotorControlInit(void) {
    InitMotor();

    mU = 70;

    last_v = (INT16S)((3927 * 4 * 4) / GetSpeed() * _RTI_P / 4);

    last_mError = last2_mError = 0;

    motorTime = lastMotorTime = absoluteTime;
}


void MotorControl(void) {

    // 修正周期约40ms为宜
    if (absoluteTime > motorTime) {
        if (absoluteTime - motorTime < 40 * 16)
            return;
    } else {
        motorTime = absoluteTime;
        return;
    }

    motorTime = absoluteTime;

    //计算速度和偏差
    tmpSpeed = GetSpeed();

    v =(INT16S)((3927 * 4 * 4) / GetSpeed() * _RTI_P / 4);


    mError = tgtSpeed - v;
    last_v = v;

    if (tgtSpeed < 200) {
        pK = 10 + (200 - tgtSpeed) * 90 / 200;
    } else {
        pK = 10;
    }

    mdU = (mError * 245 - last_mError * 350 + 125 * last2_mError) / (pK );

    last2_mError = last_mError;
    last_mError = mError;

    mU = mU + mdU;

    if (mU < 0) mU = 0;
    if (mU > 300) mU = 300;

    FrontRun((INT16U)mU);
}
