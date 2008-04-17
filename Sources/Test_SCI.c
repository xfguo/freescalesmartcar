// Test SCI

#include "includes.h"

void TestSCI(void) {
    unsigned int i = 0;
    WaitEnable();
    PORTB = 0xAA;
    DDRB = 0xFF;

    printl(1, " > > > >");
    InitSCI0();

    prints("---%d, %D, %x, %X, %s---",-3,-3,-3,-3,"few");

    for (;;) {
        Wait(10);
        PkgBegin();
        //PkgIR(1, 0x4567);

        //PkgSpeed(0x3434);

        // 发一个包要2ms
        PORTB_BIT0 = 1;
        PkgSrvAgl(0x33);
        PORTB_BIT0 = 0;

        //PkgDist(0x1234);
        PkgEnd();
    }   // WriteSCI0("09876543");
}
