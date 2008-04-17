#include "includes.h"

INT16U watchVar = 0;

void TestPWM2() {
    int i;

    StartTimeBase();

    Wait(500);

    InitPWM01();

    StartPWM01();
    for (i = 1150;i <= 1800;i+=25) {
        Wait(3500);
        SetPWM01(20000,i);
    }
}


// 控制舵机
void TestPWM3(void) {
    INT16U i;

    DDRB = 0xFF;
    PORTB = 0xA5;

    StartTimeBase();

    //Wait(3000);

    InitPWM01();

    StartPWM01();

    Wait(3000);


    for (;;) {

        for (i = 3400;i <= 5700;i+=10) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(60000,i);
            watchVar = i;
        }

        for (i = 5700;i >= 3400;i-=10) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(60000,i);
            watchVar = i;
        }

    }
}

// 当周期变化, 脉宽不变时,没有反应
void TestPWM6(void) {
    INT16U i;

    DDRB = 0xFF;
    PORTB = 0xAA;

    StartTimeBase();

    Wait(500);

    InitPWM01();

    StartPWM01();

    for (;;) {

        for (i = 0;i < 1000;i+=5) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(10000 - i,2000);
            watchVar = 10000 - i;
        }

        for (i = 0;i < 1000;i+=5) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(10000 + i,2000);
            watchVar = 10000 + i;
        }
    }
}

//控制马达
void TestPWM4(void) {
    INT8U i;

    StartTimeBase();
    DDRB = 0xFF;

    InitPWM23();

    StartPWM23();

    DDRP_DDRP4 = 1;
    DDRP_DDRP5 = 1;
    DDRP_DDRP7 = 1;

    PTP_PTP4 = 1;
    PTP_PTP5 = 0;
    PTP_PTP7 = 0;

    DDRP_DDRP2 = 1;

    for (;;) {

        for (i = 0;i < 210;i++) {
            PORTB = i;
            Wait(100);
            SetPWM23(300,i);
        }

        for (i = 210;i > 0;i--) {
            PORTB = i;
            Wait(100);
            SetPWM23(300,i);
        }

    }
}

// PWM1口出,20us高低交替
void TestPWM5(void) {

    InitPWM01();

    StartPWM01();

    SetPWM01(60000,4500);
}

// 控制舵机
/*
void TestPWM7(void) {
  INT16U i;

  DDRB = 0xFF;
  PORTB = 0xA5;

  StartTimeBase();

  Wait(500);

  InitPWM01();

  StartPWM01();
  SetPWM01(20000,560);
  Wait(5000);
  SetPWM01(20000,2420);
  Wait(5000);
  SetPWM01(20000,1500);
  Wait(5000);
    SetPWM01(20000,560);
  Wait(5000);
  SetPWM01(20000,2420);
  Wait(5000);
  SetPWM01(20000,1500);
  Wait(5000);
    SetPWM01(20000,560);
  Wait(5000);
  SetPWM01(20000,2420);
  Wait(5000);
  SetPWM01(20000,1500);
  Wait(5000);

  for(;;){

    for(i = 0;i < 1500;i+=5) {
      PORTB = ~PORTB;
      Wait(1000);
      SetPWM01(20000,500+i);
      watchVar = 500 + i;
    }

    for(i = 0;i < 1500;i+=5){
      PORTB = ~PORTB;
      Wait(1000);
      SetPWM01(20000,2500-i);
      watchVar = 2500 - i;
    }
  }
}

*/
// 马达和舵机共同作用
void TestPWM8(void) {
    INT16U i;

    StartTimeBase();

    DDRB = 0xFF;
    PORTB = 0xA5;

    InitPWM23();

    StartPWM23();

    SetPWM23(300,150);

    DDRM_DDRM4 = 1;
    DDRM_DDRM5 = 1;

    PTM_PTM4 = 0;
    PTM_PTM5 = 1;


    ////////////////////////////////////////////////////////


    Wait(1000);

    InitPWM01();

    StartPWM01();

    for (;;) {

        for (i = 3400;i <= 5700;i+=10) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(60000,i);
            watchVar = i;
        }

        for (i = 5700;i >= 3400;i-=10) {
            PORTB = ~PORTB;
            Wait(10);
            SetPWM01(60000,i);
            watchVar = i;
        }

    }
}



// 控制马达
void TestPWM9(void) {
    //INT8U i;

    StartTimeBase();
    DDRB = 0xFF;

    InitPWM45();
    InitPWM23();

    StopPWM23();
    StopPWM45();

    DDRM_DDRM4 = 1;
    DDRM_DDRM5 = 1;

    PTM_PTM4 = 0;
    PTM_PTM5 = 1;

    DDRP_DDRP0 = 0;
    PPSP_PPSP0 = 0;
    PERP_PERP0 = 1;


    for (;;) {

        if (!PTIP_PTIP0) {

            Wait(20);
            if (!PTIP_PTIP0) {
                break;
            } else {
                Wait(20);
            }
        }
    }

    StopPWM23();
    SetPWM45(300,280);
    StartPWM45();
    Wait(3000);
    StopPWM45();

}

