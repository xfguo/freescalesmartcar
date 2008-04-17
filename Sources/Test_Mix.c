// 特别的测试和不便分类的测试

#include "includes.h"
INT16U Data;
INT16U Watch_PA = 0;
INT16U Watch_IR[16];
INT8U a,b;

// 当转了一圈执行的函数
void ROUND_DET_FUNC() {
    PORTB = PACN2;

}



// 速度测试 马达和PA共同作用
void TestSpeed(void) {
    StartTimeBase();

    DDRB = 0xFF;
    PORTB = 0xA5;

    Wait(1000);

    ////////////////////////////////////
    // PA设置
    PAI_FUNC = ROUND_DET_FUNC;

    InitPA();

    ClearPA();

    StartPA();

    //////////////////////////////////
    // 马达设置

    InitPWM23();

    StartPWM23();

    SetPWM23(300,75);

    DDRM_DDRM4 = 1;
    DDRM_DDRM5 = 1;

    PTM_PTM4 = 0;
    PTM_PTM5 = 1;
}

// 测试红外发射和接受,用AD
// Data: Max -> 1023 ;  Min -> 130 ==> 45K OM

void TestIR_AD(void) {


    INT16U i,j;
    INT8U led;

    DDRB  = 0xff;
    PORTB = 0xA5;

    DDRE_BIT3 = 1;
    PORTE_BIT3 = 1;
    DDRE_BIT2 = 1;
    PORTE_BIT2 = 0;

    /* activate ADC */
    InitADC();

    /* forever */
    for (;;) {
        for (j = 0;j < 16;j++) {
            /* read ADC */
            Watch_IR[j] = ReadADC(j);  // right justified, unsigned, channel AN07


            /* determine LED bit pattern */
            for (i=0, led=0x00; i<(Watch_IR[14] & 0x3FF); i += 0x3FF/8) {

                /* assemble LED bit pattern */
                led = led<<1 | 0x01;
            }

            /* set LED array to new value */
            PORTB = (char)led;
        }

        Data = Watch_IR[6];
        a = Data % 0xFF;
        b = Data >> 8;

    }

}


// 测试测速器(原型)

/*
INT16U last_TC2, last_tc2_counter;
INT16U SpeedTimeH, SpeedTimeL;

void TestPA_TC_FUNC() {
    PORTB++;
    SpeedTimeH = tc2_counter - last_tc2_counter;
    if(TC2 < last_TC2) SpeedTimeH--;
    SpeedTimeL = TC2 - last_TC2;

    last_tc2_counter = tc2_counter;
    last_TC2 = TC2;
}

void TestPA_TC() {
    InitTC2();
    InitPA();
	ClearPA();
	PAI_FUNC = TestPA_TC_FUNC;
	last_TC2 = 0;
	last_tc2_counter = 0;

	//

	DDRB = 0xFF;
	PORTB = 0xA5;

	StartTC2();
	StartPA();
}

*/


void Test_SmallButton(void) {
    INT8U k;
    for (k = 0;k < 20;k++) {
        WaitSmallButtonPress();
        printl(1,"%d", k);

    }
}

