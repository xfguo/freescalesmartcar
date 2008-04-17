// Test ADC Modle


#include "includes.h"


INT16U Watch_AD[16];



void TestADC1(void) {


    INT16U i,j, Data;
    INT8U led;

    //StartTimeBase();
    // set port B as output (LEDs)
    DDRB  = 0xff;
    PORTB = 0xA5;

    // activate ADC
    InitADC();

    //forever
    for (;;) {
        for (j = 0;j < 16;j++) {
            //read ADC
            Data = Watch_AD[j] = ReadADC(j);  // right justified, unsigned, channel AN07

            // determine LED bit pattern
            for (i=0, led=0x00; i<(Watch_AD[15] & 0x3FF); i += 0x3FF/8) {

                // assemble LED bit pattern
                led = led<<1 | 0x01;
            }

            // set LED array to new value
            PORTB = (char)led;
        }

    }

}


// 测试同时读两套AD的值 -------------------------------------------------------

INT16U w[2][8];
INT8U t,conTime1,conTime2;

// 单独读
INT16U rad0(INT16U channel) {
    ATD0CTL5 = 	ATD0CTL5_DJM_MASK +  //数据右对齐
                (ATD0CTL5_Cx_MASK & channel); // 防止出错
    // 等待CCF0 ,即转换完成, 经计算此转换要在使 while 循环31次
    while ((ATD0STAT1&0x01)==0) {};
    return ATD0DR0;
}

// 单独读
INT16U rad1(INT16U channel) {
    ATD1CTL5 = 	ATD1CTL5_DJM_MASK +
                (ATD1CTL5_Cx_MASK & channel);
    while ((ATD1STAT1&0x01)==0) {};
    return ATD1DR0;
}

// 单独读2
void rad(INT16U channel1, INT16U channel2, INT16U * a, INT16U * b) {
    t = 0;
    ATD0CTL5 = 	ATD0CTL5_DJM_MASK +  //数据右对齐
                (ATD0CTL5_Cx_MASK & channel1); // 防止出错
    // 等待CCF0 ,即转换完成, 经计算此转换要在使 while 循环31次
    while ((ATD0STAT1&0x01)==0) {
        t++;
    };
    conTime1 = t;
    t = 0;

    *a = ATD0DR0;

    ATD1CTL5 = 	ATD1CTL5_DJM_MASK +
                (ATD1CTL5_Cx_MASK & channel2);
    while ((ATD1STAT1&0x01)==0) {
        t++;
    };
    conTime2 = t;

    *b = ATD1DR0;
}

// 合起来读
void radd(INT16U channel1, INT16U channel2, INT16U * a, INT16U * b) {
    t = 0;
    ATD0CTL5 = 	ATD0CTL5_DJM_MASK +  //数据右对齐
                (ATD0CTL5_Cx_MASK & channel1); // 防止出错
    ATD1CTL5 = 	ATD1CTL5_DJM_MASK +
                (ATD1CTL5_Cx_MASK & channel2);

    //while((ATD0STAT1&0x01)==0 && (ATD1STAT1&0x01)==0){t++;};
    while (!(ATD0STAT1_CCF0 || ATD1STAT1_CCF0)) {
        t++;
    };

    conTime1 = t;
    *a = ATD0DR0;
    *b = ATD1DR0;
}

// 测试
void TestADCDouble(void) {
    INT8U i;
    INT16U a, b;

    ATD0CTL2 = 0x80;
    ATD0CTL4 = 0x05;

    ATD1CTL2 = 0x80;
    ATD1CTL4 = 0x05;

    for (;;) {
        for (i = 0;i < 8;i++) {
            ReadADCDouble(i, i, &a, &b);
            w[0][i] = a;//rad0(i);
            w[1][i] = b;//rad1(i);
        }
    }
}

// 测试同时读两套AD的值 -------------------------------------------------------



