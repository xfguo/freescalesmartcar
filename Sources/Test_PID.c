// Test PID

#include "includes.h"

/*
INT16U FuzzyLogic(INT16U input_value);

#define FuzzySpeeder(err, acc) FuzzyLogic((INT16U)((err) << 8) + (INT16U)(acc))

INT16U mduModify = 2;
INT16S pK = 400;
INT16S tmpSpeed;
INT16S mU,mdU;
INT16S mError, mAcc, last_mError, last2_mError;
INT16S last_v = 0,v;
long motorTime, lastMotorTime;

INT16S md_Correct = 1;
*/
void Test_PID(void) {


    DDRB = 0xFF;
    PORTB = 0x1;
    WaitEnable();
    DisableButton();

    InitSpeeder();
    StartSpeeder();

    InitMotor();

    InitSCI0();

    tgtSpeed = 300;

    //time = SCI_time = absoluteTime;
    mU = 100;

    tmpSpeed = (INT16S)GetSpeed();
    last_v = (3927 * 4) / tmpSpeed * _RTI_P;

    last_mError = last2_mError = 0;

    PkgBegin();

    //for(i = 0;i < 25536;i++) {
    for (;;) {
        //  一次循环约27us

        //

        Wait(40);

        tmpSpeed = GetSpeed();

        v =(INT16S)( (3927 * 4 * 4) / GetSpeed() * _RTI_P / 4);

        mError = -(tgtSpeed - v) / 2 + 128;
        mAcc = (v - last_v) / 2 + 128;

        if (mError > 255) mError = 255;
        if (mError < 0)      mError = 0;

        if (mAcc > 255) mAcc = 255;
        if (mAcc < 0)      mAcc = 0;
                       
        last_v = v;

        mdU = FuzzySpeeder((INT8U)mError, (INT8U)mAcc);
        // 对模糊控制的一些弥补

        if (mdU == 0) {
            if (mError > 128 + mduModify) {
                mdU = -1;
            } else if (mError < 128 - mduModify) {
                mdU = 1;
            }
        }

        mU = mU + mdU;

        if (mU < 0) mU = 0;
        if (mU > 250) mU = 250;


        if (mU > 30) {
            FrontRun((INT16U)mU);
        } else {
            BackRun((INT16U)(30 - mU) * tgtSpeed / 100);
        }



        //PkgTime((INT16U)(absoluteTime - SCI_time));
        //PkgSpeed(tmpSpeed);

    }
    //PkgEnd();

}
/*
void Test_MotorCtrlInit(void) {
    InitMotor();

    mU = 100;

    tmpSpeed = (INT16S)GetSpeed();
    last_v = (3927 * 4) / (tmpSpeed + 1) ;

    last_mError = last2_mError = 0;

    motorTime = lastMotorTime = absoluteTime;
}

INT16U breakLevel = 5;

void Test_MotorCtrl(void) {

    // 修正周期约40ms为宜
    if(absoluteTime > motorTime) {
        if(absoluteTime - motorTime < 40 * 16)
            return;
    } else {
        motorTime = absoluteTime;
        return;
    }

    motorTime = absoluteTime;


    //计算速度和偏差
    tmpSpeed = GetSpeed();

    v =(INT16S)((3927 * 4 * 4) / GetSpeed() * _RTI_P / 4);

    mError = -(tgtSpeed - v)/ 2 + 128;
    mAcc = (v - last_v) / 2 + 128;

    if(mError > 255) mError = 255;
    if(mError < 0)   mError = 0;

    if(mAcc > 255) mAcc = 255;
    if(mAcc < 0)      mAcc = 0;

    last_v = v;

    // 模糊求值
    mdU =  md_Correct * FuzzySpeeder((INT8U)mError, (INT8U)mAcc);

    // 对模糊控制的一些弥补
    if(mdU == 0) {
        if(mError > 128 + mduModify) {
            mdU = -1;
        } else if (mError < 128 - mduModify){
            mdU = 1;
        }
    }

    mU = mU + mdU;

    if(mU < 0) mU = 0;
    if(mU > 250) mU = 250;


    if(mU > 30) {
        FrontRun((INT16U)mU);
    } else {
        BackRun((INT16U)(10 * breakLevel) );
    }

//    WriteSCI0Char(0xFD);
//    WriteSCI0Char((INT8U)GetDistance());
//    WriteSCI0Char((INT8U)mU);
//    WriteSCI0Char((INT8U)( GetSpeed() / 0xFF ));
//    WriteSCI0Char((INT8U)( GetSpeed() % 0xFF ));

    //TODO 加入反向的控制
}
*/
/*     没有模糊控制的PID
void Test_PID(void) {
    int i;

    WaitEnable();

    InitSpeeder();
    StartSpeeder();

    InitMotor();

    InitSCI0();

    FrontRun(50);
    Wait(1000);

    time = SCI_time = absoluteTime;
    mU = 50;

    tmpSpeed = (INT16S)GetSpeed();
    last_v = (3927 * 4) / tmpSpeed;

    PkgBegin();
    for(i = 0;i < 25536;i++) {
        Wait(2);
        tmpSpeed = (INT16S)GetSpeed();

        v = (3927 * 4) / tmpSpeed;

        mError = tgtSpeed - v;
        last_v = v;

        mdU = (mError * 245 - last_mError * 350 + 125 * last2_mError) / (pK );

        last2_mError = last_mError;
        last_mError = mError;

        mU = mU + mdU;

        if(mU < 0) mU = 0;
        if(mU > 200) mU = 200;

        FrontRun((INT16U)mU);

        PkgTime((INT16U)(absoluteTime - SCI_time));
        PkgSpeed(tmpSpeed);
    }
    PkgEnd();

}
*/

void Test_FuzzyLogic(void) {
    INT8U i ,j;

    DDRB = 0xFF;
    PORTB = 0xAA;

    InitSCI0();

    for (i = 0;i < 255;i++) {
        for (j = 0;j < 255;j++) {
            WriteSCI0Char(i);
            WriteSCI0Char(j);
            //WriteSCI0Char(FuzzySpeeder(i, j));
        }


    }

    PORTB = 0xFF;
}
