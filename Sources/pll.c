// PLL 模块

// 控制芯片的频率, 通过倍频

#include "includes.h"

//********* PLL_Init ****************
// Set PLL clock to 48 MHz, and switch 9S12 to run at this rate
// Inputs: none
// Outputs: none
// Errors: will hang if PLL does not stabilize
void InitPLL(void) {

#if PLLCLK > 48 || PLLCLK < 2
#error "Pll clock set fail"
#elif PLLCLK == 48 && OSSCLK == 16

    SYNR = PLLCLK / 2 - 1;
    REFDV = OSSCLK - 1;

    /* PLLCLK = 2 * OSCCLK * (SYNR + 1) / (REFDV + 1)
    Values above give PLLCLK of 48 MHz with 4 MHz crystal.
    (OSCCLK is Crystal Clock Frequency)                */

    while (!( CRGFLG & 0x08 )); //等待PLL稳定

    CLKSEL_PLLSEL = 1; // 切换到PLL的频率
#else
#error "Pll clock set fail"
#endif

}

