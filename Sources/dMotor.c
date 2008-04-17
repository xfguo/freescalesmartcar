// 马达控制

#include "includes.h"

#define FRONT_RUN 1
#define STOP 0
#define BACK_RUN -1

INT8S lastMotorState = STOP;

// 马达和MC33886初始化
void InitMotor(void) {

    InitPWM45();
    InitPWM23();

    DDRM_DDRM4 = 1;
    DDRM_DDRM5 = 1;

    StopPWM23();
    StopPWM45();

    PTM_PTM4 = 0;
    PTM_PTM5 = 1;

}


// ----------------------------------------------------------------------------
// 基本控制函数

// 车向前跑
void FrontRun(INT16U acc) {
    if (lastMotorState != FRONT_RUN) {
        StopPWM23();
        StartPWM45();
    }
    if (acc > 300) return;
    SetPWM45(300,acc);
    lastMotorState = FRONT_RUN;

}

// 车向后跑
void BackRun(INT16U acc) {
    if (lastMotorState != BACK_RUN) {
        StopPWM45();
        StartPWM23();
    }
    SetPWM23(300,acc);
    lastMotorState = BACK_RUN;
}

void StopRun(void) {
    StopPWM23();
    StopPWM45();
    lastMotorState = STOP;
}

// ----------------------------------------------------------------------------
// 高级控制函数

// 加速
// TODO

// 刹车
// TODO
