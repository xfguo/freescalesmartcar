// Test Speeder

#include "includes.h"

//INT16U Watch_D;

void TestSpeeder() {
    INT16U v;
    StartTimeBase();
    InitSpeeder();
    StartSpeeder();

    InitMotor();
    FrontRun(150);

    Wait(3000);

    for (;;) {
        Wait(5);
        //Watch_D = GetSpeed();
        v = 3927 * 4 / GetSpeed() * 16 ;

        printl(0, "%5D.%-2D", (INT16U)(v / 100), (INT16U)(v % 100));
        Test_ShowDistance();
    }
}

void Test_ShowDistance(void) {
    INT32U l = 393 * (INT32U)GetDistance();

    printl(1, "%5D.%-2D", (INT16U)(l / 100), (INT16U)(l % 100));
}

void Test_ClearDistance(void) {
    ClearDistanceCounter();
}

void Test_StartDistance(void) {
    InitSpeeder();
    StartSpeeder();
}
