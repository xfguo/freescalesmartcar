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

// 调试模块按键

#include "includes.h"

const INT8U btn_num = 5;
INT8U btn_flags[5];

void (*btn_func)(INT16U);

void TC1_TimeReach() {
    INT8U btn_ports[8], k;

    btn_ports[0] = PTIH_PTIH4;
    btn_ports[1] = PTIH_PTIH5;
    btn_ports[2] = PTIH_PTIH6;
    btn_ports[3] = PTIH_PTIH7;
    btn_ports[4] = PTIT_PTIT4;

    for (k = 0;k < btn_num;k++) {
        if (!btn_ports[k]) {
            if (btn_flags[k] >= 1) {
                if (btn_flags[k] == 1) {
                    if (btn_func != NULL) {
                        btn_func(k);
                    }
                    btn_flags[k]++;
                } else if (++btn_flags[k] > 20) {
                    if (btn_func != NULL) {
                        btn_func(k);
                    }
                    btn_flags[k] = 2;
                }
            } else {
                btn_flags[k] = 1;
            }
        } else {
            btn_flags[k] = 0;
        }
    }
}

// 按键的初始化
//      func_n 为按键的个数
//      func_arr 为每个按键执行函数的指针数组
void InitButton(void (*func)(INT16U)) {
    btn_func = func;

    DDRH &= 0x0F;
    PPSH &= 0x0F;
    PERH |= 0xF0;

    DDRT_DDRT4 = 0;
    PPST_PPST4 = 0;
    PERT_PERT4 = 1;

    InitTC1(TC1_TimeReach);
}

void EnableButton(void) {
    StartTC1();
}

void DisableButton(void) {
    StopTC1();
}
