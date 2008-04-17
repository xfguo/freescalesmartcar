// 测试头文件


// ****************************************************************************
// * Test Hardwares

// Test PWM
extern void TestPWM2(void);
extern void TestPWM3(void);
extern void TestPWM4(void);
extern void TestPWM5(void);

extern void TestPWM6(void);
extern void TestPWM9(void);
extern void TestPWM8(void);

// Test IR
extern void TestIR(void);

// Test PA
extern void TestPA(void);

// Test IO
extern void TestIO(void);

// Test TC
extern void TestTC1(void);
extern void TestTC2(void);

// Test ADC
extern void TestADC1(void);
extern void TestADCDouble(void);

// Test RTI
extern void TestRTI(void);

// ****************************************************************************
// * Test Devices

// Test DeBug Modle
extern void TestButton(void);

extern void TestSpeed(void);

extern void TestServo(void);

extern void TestIR_AD(void);

extern void TestPA_TC(void);

extern void TestSpeeder(void);

extern void TestIR_Senior(void);
extern void TestIR_GetWhite(void);
extern void TestIR_GetBlack(void);

extern void TestDBM_Button(void);

extern void Test_PID(void);

extern void TestMotor(void);

extern void TestIR_Collection(void);

extern void TestIR_Double(void);

extern void TestIR_Shake(void);

extern void TestServo_Angle(void);

extern void TestLCD(void);

//extern void TestMenu(void);

extern void TestSCI(void);

extern void Test_ShowDistance(void);
extern void Test_ClearDistance(void);
extern void Test_StartDistance(void);

extern void Test_SmallButton(void);

extern void TestServo_Action(void);
extern void TestServo_Turn(void);

extern void Test_UseUpBattery(void);

extern void Test_WaitError(void);

extern void Test_ServoWheelAngle(void);

extern void Test_ServoSpeed(void);

extern void Test_PID(void);

extern void Test_MotorCtrlInit(void);
extern void Test_MotorCtrl(void);

extern void Test_GetBlackAndWhite(void);

extern INT16U PosToAgl(INT16U pos);

extern void Test_IR_GetBL(void);

extern void Test_FuzzyLogic(void);

extern void Test_PrintInfoTOSCI(void);


extern INT16U maxSpeed;
extern INT16U minSpeed;
// 根据最大速度计算合适的减速距离
extern INT16U MaxStraightSpdDwnLength;
extern INT16U MinStraightSpdDwnLength;

extern INT16U ShortestHighSpeedLength;
