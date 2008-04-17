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



// 舵机控制
// 从舵机转动的角度到轮转的角度,在到车所拐弯的角度

#include "includes.h"

INT16S ServoCorrectValue = 3;

// 舵机转角角度对应表
const INT16S servo_arr[181] = {
                                  3105, 3120, 3136, 3151, 3167, 3182, 3198, 3213,
                                  3229, 3244, 3260, 3275, 3291, 3306, 3322, 3337,
                                  3353, 3368, 3384, 3399, 3415, 3430, 3446, 3461,
                                  3477, 3492, 3508, 3523, 3539, 3554, 3570, 3585,
                                  3601, 3616, 3632, 3647, 3663, 3678, 3694, 3709,
                                  3725, 3740, 3756, 3771, 3787, 3802, 3818, 3833,
                                  3849, 3864, 3880, 3895, 3911, 3926, 3942, 3957,
                                  3973, 3988, 4004, 4019, 4035, 4050, 4066, 4081,
                                  4097, 4112, 4128, 4143, 4159, 4174, 4190, 4205,
                                  4221, 4236, 4252, 4267, 4283, 4298, 4314, 4329,
                                  4345, 4360, 4376, 4391, 4407, 4422, 4438, 4453,
                                  4469, 4484, 4500, 4515, 4531, 4546, 4562, 4577,
                                  4593, 4608, 4624, 4639, 4655, 4670, 4686, 4701,
                                  4717, 4732, 4748, 4763, 4779, 4794, 4810, 4825,
                                  4841, 4856, 4872, 4887, 4903, 4918, 4934, 4949,
                                  4965, 4980, 4996, 5011, 5027, 5042, 5058, 5073,
                                  5089, 5104, 5120, 5135, 5151, 5166, 5182, 5197,
                                  5213, 5228, 5244, 5259, 5275, 5290, 5306, 5321,
                                  5337, 5352, 5368, 5383, 5399, 5414, 5430, 5445,
                                  5461, 5476, 5492, 5507, 5523, 5538, 5554, 5569,
                                  5585, 5600, 5616, 5631, 5647, 5662, 5678, 5693,
                                  5709, 5724, 5740, 5755, 5771, 5786, 5802, 5817,
                                  5833, 5848, 5864, 5879, 5895
                              };

void InitServo(void) {
    InitPWM01();
}

void StartServo(void) {
    StartPWM01();
}

void StopServo(void) {
    StopPWM01();
}

// 0 ~ 180 对应舵机 -45度到+45度
void SetServo(INT8U angle) {
    SetPWM01(60000, (INT16U)(servo_arr[angle] + (ServoCorrectValue * 20)));
    //lastServoAngle = angle;
}



/******************************************************************************
建表的C++程序
-------------------------------------------------------------------------------
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    int i = 0;
    cout << "const INT16U servo_arr[181] = {";
    for(i = 0;i <=180;i++){
        if(i) cout << ", ";
        if(!(i%8)) cout << endl << "\t";
        cout << 3105 + (5895 - 3105) * i / 180;

    }
    cout << endl << "};" << endl;
    system("PAUSE");
    return 0;
}
-------------------------------------------------------------------------------
******************************************************************************/
