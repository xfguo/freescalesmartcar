//Test Time Counter

#include "includes.h"

void test_tc1_func(void) {
    PORTB++;
    //REV_BYTE(PORTB);
}

void TestTC1(void) {
    StartTimeBase();
    DDRB = 0xFF;
    PORTB = 0xaa;

    InitTC1(test_tc1_func);
//	InitTC2();

    StartTC1();
    //Wait(2010);
    //StopTC1();


}

void TestTC2(void) {
    DDRB = 0xFF;
    PORTB = 0xaa;

    InitTC2();
    StartTC2();
}
