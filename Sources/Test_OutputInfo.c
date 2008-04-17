// Print Infomation to Serial Port

#include "includes.h"

extern INT8U crsN,crsType[10];
extern INT16U crsDist[10];
extern INT8U nowLoop;
void Test_PrintInfoTOSCI(void) {
    INT8U i;
    InitSCI0();

    prints("\n----------------\n");

    prints("crsN = %d\n", crsN);
    prints("nowLoop = %d\n", nowLoop);
    for (i = 0;i <= crsN;i++) {
        prints("%d : Dist -> %d \t Type -> %d\n", i, crsDist[i], crsType[i]);
    }
    prints("\n----------------\n");

}
