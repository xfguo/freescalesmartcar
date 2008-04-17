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

// 路径记忆

void PrintPath();
void Process(INT16U i, INT8U value);

#define RECORD 1
#define PLAY 2
extern INT8U PathMemState;

extern INT8U pathInfoN;

extern INT16U firstLoopStartDist;
extern INT16U secondLoopStartDist;

extern INT8U pathPlayN;
extern INT8U nowSpeedChgN;

extern INT8U SpeedChgN;

extern INT16U ErrorLength;
extern INT16U AccLength;

extern INT16U XSpeed;

extern INT16U needUpdateDist;

extern INT16U BendMaxSpeedBase;
extern INT16U SingleBendMaxSpeedBase;


// 计算弯道最大速度
extern INT16U GetBendMaxSpeed(INT8U x);

// 计算直道的速度
void CalcStraightSpeed(INT16U StartDist, INT16U length, INT16U BendSpeed);
// 计算弯道速度
void CalcBendSpeed(INT16U EndDist, INT16U BendSpeed, INT16U NextBendSpeed);

void PathPlayProcess(INT16U dist);

void PathPlayer(INT16U dist);

void StartPathPlay(void);
