// Test IR
#include "includes.h"


// 产生周期为2的方波
void TestIR() {
    StartTimeBase();
//	DDRB = 0xFF;
//	PORTB = 0;
    DDRP_DDRP1 = 1;
    for (;;) {
        Wait(1000);
        PTP_PTP1 = 1;
        Wait(1000);
        PTP_PTP1 = 0;
    }
}
