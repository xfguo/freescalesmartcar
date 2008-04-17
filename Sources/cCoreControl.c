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

#include "includes.h"

// 有关速度的参数
INT16S lastTgtSpeed = 100;
INT16S tgtSpeed = 100;
INT16U maxSpeed = 110;
INT16U minSpeed = 80;

// 限制距离为 多少米
INT16U DistLimit = 0;

// 绝对和相对的红外检测值
INT16U wir[nIR];
INT16U irValue[nIR];

// 黑白的参考值
INT16U whiteAvg[nIR], blackAvg[nIR];

// 中间变量
INT16U minTmp[nIR][10], maxTmp[nIR][10];

// 每次检测到的IR位置
INT16U position;

// 舵机目标位置
INT8U servoTgtAngle;

INT16U tmir;

// 十字和交叉的记录
INT8U crsN,crsType[10];
INT16U crsDist[10];

INT16U StartLineDist[4];
INT8U CrossLineN = 0, RampDistN = 0;
INT16U CrossLineDist[20];
INT16U RampDist[10];
// 当前所在的圈
INT8U nowLoop;

// 用于计算单位角度舵机拐角的平均值
INT32U PerDistSrvTotal;
INT16U PerDistSrvN;

#define swap(x, y) {(x) = (x) ^ (y);(y) = (x) ^ (y);(x) = (x) ^ (y);}


// IR 发射序列 分别向PTH, PORTE & PORTA 赋值
const INT8U irSendArr[nIR][3] = {
#if nIR == 12
                                    {0, 0, PORTA_BIT6_MASK},
                                    {0, 0, PORTA_BIT7_MASK},
                                    {PTH_PTH3_MASK, 0, 0},
                                    {PTH_PTH2_MASK, 0, 0},
                                    {PTH_PTH1_MASK, 0, 0},
                                    {PTH_PTH0_MASK, 0, 0},
                                    {0, PORTE_BIT3_MASK, 0},
                                    {0, PORTE_BIT2_MASK, 0},
                                    {0, 0, PORTA_BIT2_MASK},
                                    {0, 0, PORTA_BIT3_MASK},
                                    {0, 0, PORTA_BIT5_MASK},
                                    {0, 0, PORTA_BIT4_MASK}
#elif nIR == 8
                                    {PTH_PTH3_MASK, 0, 0},
                                    {PTH_PTH2_MASK, 0, 0},
                                    {PTH_PTH1_MASK, 0, 0},
                                    {PTH_PTH0_MASK, 0, 0},
                                    {0, PORTE_BIT3_MASK, 0},
                                    {0, PORTE_BIT2_MASK, 0},
                                    {0, 0, PORTA_BIT2_MASK},
                                    {0, 0, PORTA_BIT3_MASK},

#endif
                                };

// IR 接收管排列方式
const INT8U irRecvArr[nIR] = {
#if nIR == 12
                                 11,//0
                                 10,
                                 12,
                                 4,
                                 15,
                                 7,
                                 14,//6
                                 6,
                                 13,
                                 5,
                                 3,
                                 2
#elif nIR == 8
                                 12,
                                 14,//64,
                                 15,
                                 13,
                                 4,
                                 6,
                                 7,
                                 5
#endif
                             };

const INT8U irRevPair[nIR / 2][4] = {
#if nIR == 12
                                        {5, 0, 7, 11},
                                        {10, 1, 3, 10},
                                        {7 ,2, 6, 12},
                                        {3, 8, 4, 13},
                                        {9, 4, 5, 15},
                                        {11, 6, 2, 14}
#elif nIR == 8
                                        {7,3,5,13},
                                        {4,0,4,12},
                                        {5,1,6,14},
                                        {6,2,7,15}

#endif
                                    };


const INT8U ir_position[nIR] = {
#if nIR == 8
                                   38,
                                   63,
                                   88,
                                   113,
                                   138,
                                   163,
                                   188,
                                   213
#elif nIR == 12
                                   15,
                                   35,
                                   55,
                                   75,
                                   95,
                                   115,
                                   135,
                                   155,
                                   175,
                                   195,
                                   215,
                                   235
#endif
                               };

const INT8U speed_arr[251] = {
                                 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 1, 1, 1, 1,
                                 1, 2, 2, 2, 2, 2, 3, 3,
                                 3, 4, 4, 4, 5, 5, 5, 6,
                                 6, 6, 7, 7, 8, 8, 8, 9,
                                 9, 10, 10, 11, 11, 11, 12, 12,
                                 13, 13, 14, 14, 15, 15, 16, 16,
                                 17, 17, 18, 18, 19, 19, 20, 20,
                                 21, 21, 22, 22, 23, 23, 24, 24,
                                 25, 25, 26, 26, 27, 27, 28, 28,
                                 28, 29, 29, 30, 30, 31, 31, 31,
                                 32, 32, 33, 33, 33, 34, 34, 34,
                                 35, 35, 35, 36, 36, 36, 37, 37,
                                 37, 37, 37, 38, 38, 38, 38, 38,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 38, 38, 38, 38, 38, 37, 37,
                                 37, 37, 37, 36, 36, 36, 35, 35,
                                 35, 34, 34, 34, 33, 33, 33, 32,
                                 32, 31, 31, 31, 30, 30, 29, 29,
                                 28, 28, 28, 27, 27, 26, 26, 25,
                                 25, 24, 24, 23, 23, 22, 22, 21,
                                 21, 20, 20, 19, 19, 18, 18, 17,
                                 17, 16, 16, 15, 15, 14, 14, 13,
                                 13, 12, 12, 11, 11, 11, 10, 10,
                                 9, 9, 8, 8, 8, 7, 7, 6,
                                 6, 6, 5, 5, 5, 4, 4, 4,
                                 3, 3, 3, 2, 2, 2, 2, 2,
                                 1, 1, 1, 1, 1, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0
                             };


const INT8U servo_turn_arr[251] = {
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 52, 52, 52, 52, 52, 52, 52,
                                      52, 52, 52, 52, 53, 53, 53, 53,
                                      53, 53, 53, 53, 54, 54, 54, 54,
                                      54, 54, 54, 55, 55, 55, 55, 55,
                                      56, 56, 56, 56, 56, 57, 57, 57,
                                      57, 58, 58, 58, 58, 59, 59, 59,
                                      60, 60, 60, 61, 61, 61, 62, 62,
                                      63, 63, 63, 64, 64, 65, 65, 66,
                                      66, 67, 67, 68, 68, 69, 69, 70,
                                      71, 71, 72, 72, 73, 74, 74, 75,
                                      76, 77, 77, 78, 79, 80, 80, 81,
                                      82, 83, 84, 84, 85, 86, 87, 87,
                                      88, 89, 89, 89, 89, 89, 90, 90,
                                      91, 92, 92, 93, 94, 95, 96, 97,
                                      98, 99, 100, 101, 101, 102, 103, 104,
                                      105, 106, 106, 107, 108, 109, 109, 110,
                                      111, 111, 112, 113, 113, 114, 114, 115,
                                      115, 116, 116, 117, 117, 118, 118, 119,
                                      119, 119, 120, 120, 121, 121, 121, 122,
                                      122, 122, 122, 123, 123, 123, 124, 124,
                                      124, 124, 125, 125, 125, 125, 125, 126,
                                      126, 126, 126, 126, 127, 127, 127, 127,
                                      127, 127, 127, 127, 128, 128, 128, 128,
                                      128, 128, 128, 128, 128, 128, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129
                                  };

// 识别黑线所需的一些变量
INT16U tBlState;
INT16U tv;
INT16U ttotal,total;
INT16U minIRv, minIRn;
INT16U minIRv2, minIRn2; /* 第二小 */
INT16U maxIRv;
INT16U last_position = 90;

INT32U tdist;

// 黑线的状态
#define NORMAL 0        /* 普通黑线 */
#define LOST 1          /* 找不到 */
#define START 2         /* 起始线 */
#define CROSS 3         /* 十字交叉 */

INT16U blStateArr[4];   /* 状态数组 */
INT16U blState;         /* 状态分析结果 */
INT16U tw, tb;
void GetBlackAndWhite(void) {
    INT16U i, j;


    for (i = 0;i < nIR;i++) {
        whiteAvg[i] = 0;
        blackAvg[i] = 0;
        for (j = 0;j < 10;j++) {
            maxTmp[i][j] = 0;
            minTmp[i][j] = 1023;
        }
    }

    for (;;) {


        PORTB = 0x55;


        if (!PTIP_PTIP0) {
            // 处理
            for (i = 0;i < nIR;i++) {
                tw = 0;
                tb = 0;
                for (j = 0;j < 10;j++) {
                    if (maxTmp[i][j] != 0) {
                        tw++;
                        whiteAvg[i] += maxTmp[i][j];
                    }
                }
                whiteAvg[i] /= tw;
                for (j = 0;j < 10;j++) {
                    if (minTmp[i][j] != 1023) {
                        tb++;
                        blackAvg[i] += minTmp[i][j];
                    }
                }
                blackAvg[i] /= tb;

            }

            /** 红外值的手工修正 **/

            //whiteAvg[3] -= 36;
            //whiteAvg[2] -= 10;
            //whiteAvg[6] -= 10;

            return;
        }


        // 采集红外值
        for (i = 0;i < nIR / 2;i++) {
            irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            for (j = 0;j < 100;j++);

            ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);

        }

        irSendAllStop();

        // 找最大最小
        for (i = 0;i < nIR;i++) {
            for (j = 0;j < 10;j++) {
                if (wir[i] > maxTmp[i][j]) {
                    maxTmp[i][j] = wir[i];
                    break;
                }
            }
            for (j = 0;j < 10;j++) {
                if (wir[i] < minTmp[i][j]) {
                    minTmp[i][j] = wir[i];
                    break;
                }
            }

        }
    }
}


// 多少黑 多少白
struct _SPC_PNT {
    INT32U allWhite;
    INT32U allBlack;
    INT32U likeStart;
    INT16U dist;
}
spcPnts[30] = {{0,0,0,0}};
INT8U spcPntsN = 0;

INT16U nBlack, nWhite;

// 检测是不是同一个点, 不是则新开一个
void NewSpecialPoint(void) {
    if (GetDistance() - spcPnts[spcPntsN].dist > 8) {
        if (spcPntsN > 28) return;
        spcPntsN++;
        spcPnts[spcPntsN].allWhite = 0;
        spcPnts[spcPntsN].allBlack = 0;
        spcPnts[spcPntsN].likeStart = 0;
        spcPnts[spcPntsN].dist = GetDistance();
    }
}

void ProcessSpecialPoint(void) {
    INT16U centerMax1,centerMax2, sideMax1,sideMax2;
    INT16U i,j;
    nBlack = nWhite = 0;

    // 在路中间么
    if (LastSrvAvg > 115 || LastSrvAvg < 75)
        return;

    // 判断坡
    for (i = 0;i < nIR;i++) {
        if (wir[i] > whiteAvg[i] + (1023 - whiteAvg[i]) / 3) {
            nWhite++;
        }
    }// for

    if ( nWhite > 5 && blState == 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].allWhite++;
    }


    // 判断出发点
    centerMax1 = centerMax2 = sideMax1 = sideMax2 = 0;
#if nIR == 8
    for (j = 2;j <= 5;j++) {
#elif nIR == 12
    for (j = 4;j <= 7;j++) {
#endif
        if (centerMax1 < irValue[j]) {
            centerMax2 = centerMax1;
            centerMax1 = irValue[j];
        }
    }

#if nIR == 8
    for (j = 6;j <= 9;j++) {
        if (sideMax1 < irValue[j % 8]) {
            sideMax2 = sideMax1;
            sideMax1 = irValue[j % 8];
        }
    }
#elif nIR == 12
    for (j = 6;j <= 9;j++) {
        if (sideMax1 < irValue[j % 8 + 2]) {
            sideMax2 = sideMax1;
            sideMax1 = irValue[j % 8 + 2];
        }
    }
#endif

    if (sideMax1 + sideMax2 + 20 < centerMax1 + centerMax2 && blState > 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].likeStart++;
    }

    // 判断交叉线
    for (i = 0;i < nIR;i++) {
        if (irValue[i] < 40) {
            nBlack++;
        }
    }
    if (nBlack >= 8 && blState > 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].allBlack++;
    }
}

void PrintProcSpcResult(void) {
    INT8U i;
    prints("total = %d\n", spcPntsN);
    for (i = 1;i <= spcPntsN;i++) {
        prints("w = %d\tb = %d\ts = %d\tdist = %d\n",
               (INT16U)spcPnts[i].allWhite, (INT16U)spcPnts[i].allBlack, (INT16U)spcPnts[i].likeStart, spcPnts[i].dist);
    }
}




void ProcStartLine(void) {
    nowLoop++;
    switch (nowLoop) {
    case 1:
        PORTB = ~0x22;

        StartLineDist[0] = GetDistance();
        break;
    case 2:
        StartLineDist[1] = GetDistance();

        PORTB = 0xA5;

        // 跑完第二圈后停下
        DistLimit = ( GetDistance() + (StartLineDist[1] - StartLineDist[0]) ) / 25 + 4;
        /** 在这里触发路径播放 **/
        StartPathPlay();

        //maxSpeed = 110;
        //minSpeed = 75;


        break;
    case 3:
        // 到第三圈后停下
        StartLineDist[2] = GetDistance();

        PORTB = 0x81;
        //DistLimit = GetDistance() / 25 + 2;
        break;
    default:
        break;
    }
}
void ProcRamp(void) {
    PORTB = 0x77;

    RampDist[RampDistN] = GetDistance();
    RampDistN++;

}
void ProcCrossLine(void) {
    PORTB = ~PORTB;

    CrossLineDist[CrossLineN] = GetDistance();
    CrossLineN++;
}

// 已经检查过的点
INT8U chkedPntsN = 0;
INT16U lastAllWhiteDist = 0;
void CheckNewPoint(void) {
    // 检查是不是有没有处理的新点
    if (chkedPntsN < spcPntsN && spcPnts[spcPntsN].dist + 4 < GetDistance() ) {
        if (lastAllWhiteDist == 0 || lastAllWhiteDist + 70 < GetDistance()) {
            if (spcPnts[spcPntsN].allWhite > 0) {
                // 进入坡道
                //PORTB = ~(1);
                lastAllWhiteDist = GetDistance();

                ProcRamp();

            }

            if (spcPnts[spcPntsN].allBlack > 0 || spcPnts[spcPntsN].likeStart > 0) {
                // 此处 乘3 因为allBlack 往往比 likeStart 大很多
                if (spcPnts[spcPntsN].allBlack > spcPnts[spcPntsN].likeStart * 3) {
                    // 交叉点
                    //PORTB = ~PORTB;
                    ProcCrossLine();

                } else {
                    // 出发线
                    //PORTB = 0x9;
                    ProcStartLine();


                }
            }
        }
        chkedPntsN++;
    }
}

INT16U xo = 100;
INT16U CenterOffset(INT16U bl) {
    if ( bl >= xo ) {
        return 125 + ( bl - xo ) * 125 / (250 - xo);
    } else {
        return 125 - (xo - bl) * 125 / xo;
    }
}

// 核心进程
void CoreControl(void) {
    INT16U i = 0, j;
    INT8U tpls = 0;
//    INT16U tmp;

    // 串口发送的状态, 和串口字
    //INT8U sciState = 0;//, sciChar;

    StartSpeeder();
    ClearDistanceCounter();

    MotorControlInit();

    crsN = 0;
    nowLoop = 0;


    FOREVER() {
        //PORTB = ~PORTB;

        // 判断小按键是否按下, 是则进入菜单
        if (!PTIP_PTIP0) {
            WaitEnable();
            Wait(20);
            if (!PTIP_PTIP0) {
                StopRun();
                StartMenu();
                WaitSmallButtonPress();
                Wait(1500);
            }
        }


        // 采集红外值
        for (i = 0;i < nIR / 2;i++) {
            irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            for (j = 0;j < 100;j++);

            ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);
        }

        // 将各值归一化
        for (i = 0;i < nIR;i++) {
            if (whiteAvg[i] < wir[i]) {
                irValue[i] = 100;
            } else if (blackAvg[i] > wir[i]) {
                irValue[i] = 0;
            } else {
                irValue[i] = (100 * (wir[i] - blackAvg[i])) / ( whiteAvg[i] - blackAvg[i]);
            }
        }

        /** 红外值的手工修正 **/
        //irValue[2] = irValue[2] * 100 / 94;
        //irValue[3] = irValue[3] * 100 / 86;
        //irValue[6] = irValue[6] * 100 / 94;


        // 找最低
        minIRv = 100;
        maxIRv = 0;
        ttotal = 0;
        for (i = 0;i < nIR;i++) {
            if ( minIRv > irValue[i] ) {
                minIRn2 = minIRn;
                minIRn = i;
                minIRv2 = minIRv;
                minIRv = irValue[i];
            }

            if ( maxIRv < irValue[i] ) {
                maxIRv = irValue[i];
            }

            ttotal+= irValue[i];
        }
        total = ttotal;



        // 求出精确位置
        if ( minIRn <= (nIR - 1 - 1) && minIRn >= 1) {
            position = ir_position[minIRn - 1] + irValue[minIRn - 1] * IR_SPACE_BETWEEN / (irValue[minIRn - 1] + irValue[minIRn + 1]);
        } else if (minIRn == 0) {
            if (irValue[1] >= 90) {
                position = ir_position[0] - (IR_SPACE_BETWEEN / 2) * irValue[0] / (irValue[0] + irValue[1]);
            } else {
                position = ir_position[0] + (IR_SPACE_BETWEEN / 2) * irValue[0] / (irValue[0] + irValue[1]);
            }
        } else {
            if (irValue[nIR - 1 - 1] >= 92/** TODO 这样合适么? **/) {
                position = ir_position[nIR - 1] + (IR_SPACE_BETWEEN / 2) * irValue[nIR - 1] / (irValue[nIR - 1 - 1] + irValue[nIR - 1]);
            } else {
                position = ir_position[nIR - 1] - (IR_SPACE_BETWEEN / 2) * irValue[nIR - 1] / (irValue[nIR - 1 - 1] + irValue[nIR - 1]);
            }
        }

        // 排错, 理论上前后两次之间的值相差不应超过15

        if ( ((position > last_position)?(position - last_position):(last_position - position)) > 60) {
            if (nowLoop == 1) {
                if (GetDistance() - lastAllWhiteDist > 50)
                    position = last_position;
            } else if (nowLoop == 2) {
                if (GetDistance() - (StartLineDist[1] - StartLineDist[0]) - lastAllWhiteDist > 50)
                    position = last_position;
            }

        }


        // 类模糊法判断各种路面情况
        /** TODO 各各变量的权值还可以调节(通过数据), 变量数还可以增加 **/
        //情况                  minIRv              minIRv2             maxIRv              total
        blStateArr[NORMAL]  =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   total / nIR;
        blStateArr[LOST]    =   minIRv         +    minIRv2         +   maxIRv          +   (100 - total / nIR);
        blStateArr[CROSS]   =   (100 - minIRv) +    (100 - minIRv2) +   (100 - maxIRv)  +   (100 - total / nIR);
        blStateArr[START]   =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   (100 - total / nIR);

        // 找最有可能的情况
        blState = NORMAL;
        for (i = 1;i < 4;i++) {
            if (blStateArr[blState] < blStateArr[i]) {
                blState = i;
            }
        }

        tBlState = blState;


        // 根据判断结果作出相应
        if ( blState != NORMAL) {


            ProcessSpecialPoint();
            if (GetDistance() - lastAllWhiteDist < 50) {
                last_position = position;
            } else {
                position = last_position;
            }


        } else {
            last_position = position;
        }

        // 得出舵机转角
#if nIR == 8
        // 与以前的匹配
        if (position > 125) {
            position = 125 + (position - 125) * 220 / 175;
        } else {
            position = 125 - (125 - position) * 220 / 175;
        }
#endif
        /** TODO 使用一个函数发生器来为舵机提供转角 **/
        servoTgtAngle = (INT8U)PosToAgl(position);

        //累加舵机角度
        if (PerDistSrvTotal < 0xFFFFFF && PerDistSrvN < 0xFFF0 && servoTgtAngle < 130 && servoTgtAngle > 50 ) {
            PerDistSrvTotal += servoTgtAngle;
            PerDistSrvN++;
        }


        // 简单转速控制
        tgtSpeed = (INT16S)(minSpeed + (maxSpeed - minSpeed) * (speed_arr[position] ) / 40);


        if (tgtSpeed < 50 || tgtSpeed > XSpeed) {
            tgtSpeed = lastTgtSpeed;
        } else {
            lastTgtSpeed = tgtSpeed;
        }


        // 转动舵机
        ServoControl();

        // 转动马达, 有限制距离的
        if (DistLimit != 0 && DistLimit * 25 < GetDistance() ) {
            StopRun();
        } else {
            MotorControl();
        }

        tdist = GetDistance();

        /*if(SCI0SR1 & 0x80) {
            switch(sciState) {
                case 0:
                    sciChar = 0xFE;
                    break;
                case 1:
                    sciChar = (INT8U)position;
                    break;
                case 2:
                    sciChar = (INT8U)servoTgtAngle;
                    break;
                case 3:
                    sciChar = ((INT8U)((3927 * 4) / GetSpeed())  * _RTI_P )& 0xFF;
                    break;
                case 4:
                    sciChar = (INT8U)(GetDistance());
                default:  
                    break;            
            }
            sciState = (INT16U)(sciState + 1) % 5;
            SCI0DRL = sciChar;
        } */
        ///////////////////////////////////////////////////////////////////////
    }

}

void StartCore(void) {
    PORTB = 0xAA;
    DDRB = 0xFF;

    //  初始化小按键
    DDRP_DDRP0 = 0;
    PPSP_PPSP0 = 0;
    PERP_PERP0 = 1;

    /********************************/
    /* 初始化 */
    InitServo();
    StartServo();
    SetServo(90);

    InitSpeeder();

    irInit();

    InitADC();

    InitSCI0();

    WaitEnable();

    /*******************************/

    GetBlackAndWhite();

    PORTB = 0x55;

    Wait(1500);

    PORTB = 0xAA;

    while (PTIP_PTIP0);

    PORTB = 0x5A;

    Wait(1000);

    PORTB = 0xA5;

    CoreControl();

}
