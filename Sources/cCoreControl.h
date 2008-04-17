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

// 黑线识别

// 红外发射和接受传感器的数量
#define nIR 12

#if nIR == 8
#define IR_SPACE_BETWEEN 60
#elif nIR == 12
#define IR_SPACE_BETWEEN 40
#endif

extern INT16S tgtSpeed;
extern INT16U maxSpeed;
extern INT16U minSpeed;
extern INT16U DistLimit;

extern INT32U PerDistSrvTotal;
extern INT16U PerDistSrvN;

extern INT8U servoTgtAngle;

extern INT16U wir[nIR];
extern INT16U irValue[nIR];

extern INT16U whiteAvg[nIR], blackAvg[nIR];

extern INT16U minTmp[nIR][10], maxTmp[nIR][10];

extern INT16U tmir;

// 十字和交叉的记录
extern INT8U crsN,crsType[10];
extern INT16U crsDist[10];

extern INT8U LastSrvAvg;

extern INT16U StartLineDist[4];
extern INT8U CrossLineN, RampDistN;
extern INT16U CrossLineDist[20];
extern INT16U RampDist[10];

// 当前所在的圈
extern INT8U nowLoop;

void GetBlackAndWhite(void);

void CoreControl(void);

void StartCore(void);

void PrintProcSpcResult(void);

void CheckNewPoint(void);

// IR 发射序列 分别向PTH, PORTE & PORTA 赋值
extern const INT8U irSendArr[nIR][3];

// IR 接收管排列方式
extern const INT8U irRecvArr[nIR];

extern const INT8U irRevPair[nIR / 2][4];

extern const INT8U ir_position[nIR];

extern const INT8U speed_arr[251];

extern const INT8U servo_turn_arr[251] ;

#define irInit() { \
                        DDRA = DDRA_BIT2_MASK + DDRA_BIT3_MASK     \
                                + DDRA_BIT4_MASK + DDRA_BIT5_MASK  \
                                + DDRA_BIT7_MASK + DDRA_BIT6_MASK; \
                        DDRH = 0xF;                   \
                        DDRE = DDRE_BIT3_MASK + DDRE_BIT2_MASK; \
                    }


#define irSend(x)   { \
                        PTH = irSendArr[(x)][0]; \
                        PORTE = irSendArr[(x)][1]; \
                        PORTA = irSendArr[(x)][2]; \
                    }

#define irSendDouble(x,y)   { \
                        PTH = irSendArr[(x)][0] + irSendArr[(y)][0]; \
                        PORTE = irSendArr[(x)][1] + irSendArr[(y)][1]; \
                        PORTA = irSendArr[(x)][2] + irSendArr[(y)][2]; \
                    }

#define irSendAllStop() {PTH = 0;PORTA = 3;PORTE_BIT2 = 0;PORTE_BIT3 = 0;}

