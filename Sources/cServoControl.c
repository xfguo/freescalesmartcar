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

// 舵机转角控制
#include "includes.h"

INT16U ruleSelect = 0;

const INT16U SrvRule[2][7][3] = {
                                    {
                                        {
                                            0, 40, 56
                                        },
                                        {41, 49, 203},
                                        {80, 80, 56},
                                        {125, 90, 50},
                                        {171, 99, 276},
                                        {209, 140, 62},
                                        {250, 150, 0}
                                    }
                                    ,{
                                        {0, 40, 12},
                                        {41, 42, 184},
                                        {102, 86, 44},
                                        {125, 90, 23},
                                        {179, 95, 286},
                                        {221, 142, 70},
                                        {250, 150, 0}
                                    },
                                };

INT16U PosToAgl(INT16U pos) {
    INT16U k;

    if (ruleSelect == 0) {
        return servo_turn_arr[pos];
    }

    k = 2;

    while (!(pos < SrvRule[ruleSelect - 1][k+1][0] && pos >= SrvRule[ruleSelect - 1][k][0])) {
        if (pos >= SrvRule[ruleSelect - 1][k][0]) {
            k++;
        } else {
            k--;
        }
    }

    return SrvRule[ruleSelect - 1][k][1] + (SrvRule[ruleSelect - 1][k][2] * (pos - SrvRule[ruleSelect - 1][k][0]) >> 8);
}

void ServoControl(void) {
    /** TODO 更优的控制 **/
    SetServo((INT8U)servoTgtAngle + 2);
}
