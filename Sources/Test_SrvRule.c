// ¶æ»ú×ª½Ç¿ØÖÆ
#include "includes.h"
/*
INT16U ruleSelect = 0;
INT16U k;
INT16U tp;
extern const INT8U servo_turn_arr[251];
const INT16U SrvRule[2][7][3] = {
    {
        {0, 40, 56},
        {41, 49, 203},
        {80, 80, 56},
        {125, 90, 50},
        {171, 99, 276},
        {209, 140, 62},
        {250, 150, 0}
    },{
        {0, 40, 12},
        {41, 42, 184},
        {102, 86, 44},
        {125, 90, 23},
        {179, 95, 286},
        {221, 142, 70},
        {250, 150, 0}
    },
};

INT16U m,n,x;

INT16U PosToAgl(INT16U pos) {
    if(ruleSelect == 0) {
        return servo_turn_arr[pos];
    }
    k = 2;
    ruleSelect--;
    while(!(pos < SrvRule[ruleSelect][k+1][0] && pos >= SrvRule[ruleSelect][k][0])) {
        if(pos >= SrvRule[ruleSelect][k][0]) {
            k++;
        } else {
            k--;
        }
    }
    x = pos;
    m = SrvRule[ruleSelect][k][1];
    n = SrvRule[ruleSelect][k][2] * (pos - SrvRule[ruleSelect][k][0]) >> 8;


    ruleSelect++;
    return SrvRule[ruleSelect][k][1] + (SrvRule[ruleSelect][k][2] * (pos - SrvRule[ruleSelect][k][0]) >> 8);
}*/
