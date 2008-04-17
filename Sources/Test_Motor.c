// Test Motor

#include "includes.h"

void TestMotor(void) {
    INT8U i;
    StartTimeBase();
    InitMotor();
    for (i = 0;i < 3;i++) {
        FrontRun(100);
        Wait(1000);
        StopRun();
        Wait(1000);
        BackRun(100);
        Wait(1000);
    }

    StopRun();
}

void Test_UseUpBattery(void) {
    WaitEnable();
    InitMotor();
    for (;;) {
        FrontRun(200);
        Wait(60000);
        StopRun();
        Wait(10000);
    }
}
