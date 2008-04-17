// Test GPIO
#include "includes.h"

// ²âÊÔ°´¼ü
void TestIO(void) {
    INT8U flag=0;

    StartTimeBase();

    PORTB = 0x0;
    DDRB = 0xFF;


    DDRM = 0x00;
    PPSM = 0x00;
    PERM = 0xFF;

    for (;;) {
        if (!PTIM_PTIM0) {
            if (flag >= 1) {
                if (flag == 1) {
                    PORTB++;
                    flag++;
                } else {
                    if (++flag > 50) {
                        PORTB++;
                        flag = 2;
                    }
                }
            } else {
                flag = 1;
            }
        } else {
            flag = 0;
        }
        Wait(19);
    }
}

