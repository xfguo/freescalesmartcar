// Test LCD

#include "includes.h"
#include <stdlib.h>

void TestLCD(void) {
    int i;

    WaitEnable();
    InitLCD();

    for (i = 0;i < 0xff;i+=10) {
        printl(0,">%D<", i);
        printl(1,">%X<", i);
    }

    printl(0,">%6d<", 1234);
    printl(1,">%-6d<", 1234);

    ClearLCD();
    Wait(1000);


    printl(0,"%4d%4s", 1234, "abcd");
    printl(1,"%4d%4s<", 34, "cd");


    Wait(1000);

    printl(0,"%4x%4s", 1234, "abcd");
    printl(1,"%4x%4s<", 34, "cd");


    Wait(1000);

    printl(0,"%-4d%-4s", 1234, "abcd");
    printl(1,"%-4d%-4s<", 34, "cd");

}
