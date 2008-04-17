// Test RTI

#include "includes.h"

// 测试RTI
// 注意:RTI 使用晶振频率 与 PLL无关
// RTICTL = 0x1B 时 周期约为1ms
void TestRTI(void) {
    StartTimeBase();
    PORTB = 0xAA;
    DDRB = 0xFF;
    for (;;) {

        Wait(1);
        PORTB = ~PORTB;
    }
}

void Test_WaitError(void) {
    StartTimeBase();
    StartTimeBase();

    DDRB = 0xff;
    for (;;) {
        Wait(1000);
        PORTB = 0xAA;

        Wait(1000);
        PORTB = 0x55;

    }

}

