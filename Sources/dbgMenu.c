/*
 * Copyright (c) 1998, Xiongfei Guo, Shanghai Second Polytechnic University 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Shanghai Second Polytechnic University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// 菜单模块

#include "includes.h"


#define pEND 0
#define pVALUE 1
#define pFUNC 2
#define pMENU 3
#define pVINT16U 4
#define pVINT16S 5
#define pVINT8U 6
#define pVINT8S 7


#define MENU_END {"", pEND, NULL}

typedef struct {
    char name[9];
    int pointTo;
    void  * far p;
} Menu;
            
int testInt = 5;
int testInt2 = 3;

INT8U int8u = 0;
INT8S int8s = 0;
INT16U int16u = 0;
INT16S int16s = 0;   

//extern INT8U tSrvAgl;
extern INT16S tgtSpeed;
extern INT16U mduModify;

extern INT16U maxSpeed, minSpeed;

extern INT16U DistLimit;

extern INT16U ruleSelect;

extern INT16S ServoCorrectValue;

extern INT16U breakLevel;

extern INT16S md_Correct;

/******************* 各菜单的定义 ********************************************/

/*
const Menu ServoCtl[5] = { 
    {"StartSrv",    pFUNC,      (void*)StartSrv},
    {"ChgServo",    pVINT16U,   (void*)servoAgl},
    {"ServoMAX",    pVINT16U,   (void*)PWM01MAX},
    {"ServoMIN",    pVINT16U,   (void*)PWM01MIN},
    MENU_END
};
*/

const Menu mTestPID[] = { 
    {"Test PID",        pFUNC,   (void*)Test_PID},
    {"      pK",        pVINT16U,   (void*)&pK}, 
    {"TestFuzy",        pFUNC,      (void*)Test_FuzzyLogic},        
    MENU_END     
};

const Menu TestChgN[] = { 
    {"INT16U  ",      pVINT16U,   (void*)&int16u},
    {"INT16S  ",      pVINT16S,   (void*)&int16s},
    {"INT8U   ",       pVINT8U,    (void*)&int8u},
    {"INT8S   ",       pVINT8S,    (void*)&int8s},
    MENU_END     
};


const Menu DistanceMenu[] = {
    {"ShowDist",    pFUNC,      (void*)Test_ShowDistance}, 
    {"StartDis",    pFUNC,      (void*)Test_StartDistance},
    {"ClearDis",    pFUNC,      (void*)Test_ClearDistance}, 
    {"TestSped",    pFUNC,      (void*)TestSpeeder},        
    MENU_END        
};

const Menu SomeTest[] = { 
    {"TestSBtn",    pFUNC,      (void*)Test_SmallButton},
    {"TestPWM6",    pFUNC,      (void*)TestPWM6},
    MENU_END    
};

const Menu SrvAngle[] = { 
//    {"SrvAngle",    pVINT8U,    (void*)&tSrvAgl},
//    {"TsSrvAgl",    pFUNC,      (void*)Test_ServoWheelAngle},
    MENU_END
};
const Menu SpeedLmt[] = { 
    {"maxSpeed",    pVINT16U,    (void*)&maxSpeed},
    {"minSpeed",    pVINT16U,    (void*)&minSpeed},
    MENU_END
};

const Menu MainMenu[] = { 
    {"SmartCar",    pFUNC,      (void*)StartCore}, 
    {"1",      pFUNC,     (void*)ChgSet1},
    {"2",      pFUNC,     (void*)ChgSet2},
    {"3",      pFUNC,     (void*)ChgSet3},
    {"PrintPth",    pFUNC,      (void*)PrintPath},
    {"PrntSPCP",    pFUNC,      (void*)PrintProcSpcResult},
    {"IR v Col",    pFUNC,      (void*)TestIR_Collection},
    {"SpeedLmt",    pMENU,      (void*)SpeedLmt}, 
    //{"Motor V",     pVINT16U,    (void*)&motorV},
    //{"tgtSpeed",    pVINT16S,    (void*)&tgtSpeed},
    {"Dist Lmt",    pVINT16U,    (void*)&DistLimit}, 
            
    {"duModify",    pVINT16U,    (void*)&mduModify},
    {"md_Corrt",    pVINT16S,   (void*)&md_Correct},        
    {"Rule NO.",    pVINT16U,   (void*)&ruleSelect},        
    {"brkLevel",    pVINT16U,   (void*)&breakLevel},        
    {"SrvCorct",    pVINT16S,   (void*)&ServoCorrectValue},
    {"OutpInfo",    pFUNC,      (void*)&Test_PrintInfoTOSCI},        
    {"Distance",    pMENU,      (void*)DistanceMenu}, 
    {"Test-PID",    pMENU,      (void*)mTestPID},
    
    {"SomeTest",    pMENU,      (void*)SomeTest}, 
    {"SrvoTurn",    pFUNC,      (void*)TestServo_Turn},        
    {"UseUpBat",    pFUNC,      (void*)Test_UseUpBattery}, 
    {"TestMenu",    pFUNC,      (void*)Test_WaitError},
    {"TestSCI ",    pFUNC,      (void*)TestSCI },
//    {"SrvAngle",    pMENU,      (void*)SrvAngle},        
    {"TestChgN",    pMENU,      (void*)TestChgN}, 
    {"ServoAct",    pFUNC,      (void*)TestServo_Action}, 
    //{"GetBL&WH",    pFUNC,      (void*)Test_GetBlackAndWhite}, 
    //{"Car GO >",    pFUNC,      (void*)Test_IR_GetBL}, 
    
    MENU_END
};
  
Menu * mp = (Menu *)MainMenu ,*  lastmp = (Menu*)MainMenu;
int mn = 0, lastmn = 0;

// 修改和显示所得到的值 
void changeValue(void * num, int d, int type) {
    switch(type) {
        case pVALUE:
        case pVINT16S:
            printl(1, "%8d", (*((INT16S *)num) += (INT16S)d));
            break;
        case pVINT16U:
            printl(1, "%8D", (*((INT16U *)num) += (INT16U)d));
            break;
        case pVINT8U:
            printl(1, "%8d", (INT16U)(*((INT8U *)num) += (INT8U)d));
            break;
        case pVINT8S:
            printl(1, "%8d", (INT16S)(*((INT8S *)num) += (INT8S)d));            
            break;
        default:
            break;
    }
}

void MenuSelect(INT16U btn) {    
    switch(btn) {
// 左上
        case 0:
            // 上一个 
            if(mn - 1 >= 0) {
                mn-- ;
            } else {
                // 到0则到最后一个 
                if(mn > 0) while(mp[++mn + 1].pointTo != pEND);                    
            }
            
            // 显示当前项状态
            if( mp[mn].pointTo == pVALUE || 
                (mp[mn].pointTo >= 4 && mp[mn].pointTo <=7) ) {
                changeValue(mp[mn].p, 0, mp[mn].pointTo);    
            } else if(mp[mn].pointTo == pFUNC) {
                printl(1,"    >-->");
            } else {
                printl(1,"      +>");                
            }
                                
            break;
// 左下            
        case 4:
            // 下一个 
            if(mp[mn + 1].pointTo) {
                mn++; 
            } else {
                mn = 0;
            }
            
            // 显示当前项状态
            if( mp[mn].pointTo == pVALUE || 
                (mp[mn].pointTo >= 4 && mp[mn].pointTo <=7) ) {
                changeValue(mp[mn].p, 0, mp[mn].pointTo);    
            } else if(mp[mn].pointTo == pFUNC) {
                printl(1,"    >-->");
            } else {
                printl(1,"      +>");                
            }
            
                
        
            break;
// 右上            
        case 2:
            if(mp[mn].pointTo == pVALUE || 
                (mp[mn].pointTo >= 4 && mp[mn].pointTo <=7) ) {
                changeValue(mp[mn].p, -1, mp[mn].pointTo);    
            }
            break;            
// 右下           
        case 1:
            // 下一级 
            switch (mp[mn].pointTo) {
                case pVALUE:
                    changeValue(mp[mn].p, 1, pVINT16S);
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                    changeValue(mp[mn].p, 1, mp[mn].pointTo);
                    break;
                case pFUNC:
                    ( (void (*)(void)) mp[mn].p)();
                    break;
                case pMENU:
                    lastmp = mp;
                    lastmn = mn;
                    
                    // 记录上一级菜单 
                    mp = (Menu *)mp[mn].p;
                    mn = 0;
            
                    // 显示当前项状态
                    if( mp[mn].pointTo == pVALUE || 
                        (mp[mn].pointTo >= 4 && mp[mn].pointTo <=7) ) {
                        changeValue(mp[mn].p, 0, mp[mn].pointTo);    
                    } else if(mp[mn].pointTo == pFUNC) {
                        printl(1,"    >-->");
                    } else {
                        printl(1,"      +>");                
                    }
            
                    break;
                default:
                    break;
            }
            
            break;
// 中        
        case 3:
            // 上一级菜单 
            mp = lastmp;
            mn = lastmn;
            break;
            
        default:
            break;
        
    }
    printl(0,"%8s", mp[mn].name);
    
      
}

void StartMenu(void){
    InitLCD();
    InitButton(MenuSelect);
    EnableButton(); 

    printl(0, "%s", mp[mn].name);   
}
