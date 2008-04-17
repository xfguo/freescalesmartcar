// 配置

#include "includes.h"

/*void ChgSet(void) {

} */

// 拼第一圈
void ChgSet1(void) {

    maxSpeed = 165;
    minSpeed = 110;

    // 直道误差距离
    StrightErrorLength = 3;

    // 最短高速行驶距离
    ShortestHighSpeedLength = 14;

    // 在一定长的直道上的最小速度
    MSpeed = 130;

    // 根据最大速度计算合适的减速距离
    MaxStraightSpdDwnLength = 40;
    MinStraightSpdDwnLength = 10;

    // 弯道最大速度基
    BendMaxSpeedBase = 115;
    SingleBendMaxSpeedBase = 130;

}

void ChgSet2(void) {
    maxSpeed = 155;
    minSpeed = 115;

    // 直道误差距离
    StrightErrorLength = 3;

    // 最短高速行驶距离
    ShortestHighSpeedLength = 14;

    // 在一定长的直道上的最小速度
    MSpeed = 130;

    // 根据最大速度计算合适的减速距离
    MaxStraightSpdDwnLength = 40;
    MinStraightSpdDwnLength = 10;

    // 弯道最大速度基
    BendMaxSpeedBase = 110;
    SingleBendMaxSpeedBase = 125;

}

// 稍猛一点的
void ChgSet3(void) {
    maxSpeed = 155;
    minSpeed = 110;

    // 直道误差距离
    StrightErrorLength = 2;

    // 最短高速行驶距离
    ShortestHighSpeedLength = 8;

    // 在一定长的直道上的最小速度
    MSpeed = 135;

    // 根据最大速度计算合适的减速距离
    MaxStraightSpdDwnLength = 40;
    MinStraightSpdDwnLength = 10;

    // 弯道最大速度基
    BendMaxSpeedBase = 110;
    SingleBendMaxSpeedBase = 130;
}
