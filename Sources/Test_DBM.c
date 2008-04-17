// 测试 调试模块 按键 液晶等
#include "includes.h"

/* ******************** 按键模块(原型) **************************************
INT8U btn_num;
INT8U btn_flags[8];

void (*btn_func[8])(void);

void TC1_TimeReach() {
    INT8U btn_ports[8], k;

	btn_ports[0] = PTIH_PTIH4;
	btn_ports[1] = PTIH_PTIH5;
	btn_ports[2] = PTIH_PTIH6;
	btn_ports[3] = PTIH_PTIH7;
    //btn_ports[4] = PORTE_BIT7;

	for(k = 0;k < btn_num;k++) {
		if(!btn_ports[k]){
			if(btn_flags[k] >= 1) {
				if(btn_flags[k] == 1) {
					if(btn_func[k] != NULL) {
						btn_func[k]();
					}
					btn_flags[k]++;
				} else if(++btn_flags[k] > 50) {
					if(btn_func[k] != NULL) {
						btn_func[k]();
					}
					btn_flags[k] = 2;
				}
			} else {
				btn_flags[k] = 1;
			}
		} else {
			btn_flags[k] = 0;
		}
	}
}

void TestBtnFunc() {
	PORTB++;
}

void TestButton(void) {
	INT8U k;

	btn_num = 4;
	for(k = 0;k < btn_num;k++) {
		btn_flags[k] = 0;
		btn_func[k] = NULL;
	}

	btn_func[0] = TestBtnFunc;
	btn_func[1] = TestBtnFunc;
	btn_func[2] = TestBtnFunc;
	btn_func[3] = TestBtnFunc;
	//btn_func[4] = TestBtnFunc;

	DDRB = 0xFF;
	PORTB = 0xAA;

	DDRH = 0x00;
	PPSH = 0x00;
	PERH = 0xFF;

	InitTC1(TC1_TimeReach);
	StartTC1();

}*/

void testBtnFunc(INT16U n) {

    //DisableButton();
    printl(0,"%d", n);
    PORTB = ~(INT8U)(1 << n);
}

void TestDBM_Button(void) {

    PORTB = 0xAA;
    DDRB = 0xFF;

    InitLCD();
    InitButton(testBtnFunc);
    EnableButton();

}
