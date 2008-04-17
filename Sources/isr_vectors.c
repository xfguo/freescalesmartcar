#include "includes.h"

extern void near _Startup(void);       /* Startup routine */
extern void near RTI_ISR(void);
extern void near PAI_ISR(void);
extern void near PAOVI_ISR(void);
extern void near TC1_ISR(void);
extern void near TC2_ISR(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
ISR UnimplementedISR(void) {
    /* Unimplemented ISRs trap.*/
    asm BGND;
}

typedef void (*near tIsrFunc)(void);
const tIsrFunc _vect[] @0xFF80 = {     /* Interrupt table */
                                     UnimplementedISR,                 /* vector 63 */
                                     UnimplementedISR,                 /* vector 62 */
                                     UnimplementedISR,                 /* vector 61 */
                                     UnimplementedISR,                 /* vector 60 */
                                     UnimplementedISR,                 /* vector 59 */
                                     UnimplementedISR,                 /* vector 58 */
                                     UnimplementedISR,                 /* vector 57 */
                                     UnimplementedISR,                 /* vector 56 */
                                     UnimplementedISR,                 /* vector 55 */
                                     UnimplementedISR,                 /* vector 54 */
                                     UnimplementedISR,                 /* vector 53 */
                                     UnimplementedISR,                 /* vector 52 */
                                     UnimplementedISR,                 /* vector 51 */
                                     UnimplementedISR,                 /* vector 50 */
                                     UnimplementedISR,                 /* vector 49 */
                                     UnimplementedISR,                 /* vector 48 */
                                     UnimplementedISR,                 /* vector 47 */
                                     UnimplementedISR,                 /* vector 46 */
                                     UnimplementedISR,                 /* vector 45 */
                                     UnimplementedISR,                 /* vector 44 */
                                     UnimplementedISR,                 /* vector 43 */
                                     UnimplementedISR,                 /* vector 42 */
                                     UnimplementedISR,                 /* vector 41 */
                                     UnimplementedISR,                 /* vector 40 */
                                     UnimplementedISR,                 /* vector 39 */
                                     UnimplementedISR,                 /* vector 38 */
                                     UnimplementedISR,                 /* vector 37 */
                                     UnimplementedISR,                 /* vector 36 */
                                     UnimplementedISR,                 /* vector 35 */
                                     UnimplementedISR,                 /* vector 34 */
                                     UnimplementedISR,                 /* vector 33 */
                                     UnimplementedISR,                 /* vector 32 */
                                     UnimplementedISR,                 /* vector 31 */
                                     UnimplementedISR,                 /* vector 30 */
                                     UnimplementedISR,                 /* vector 29 */
                                     UnimplementedISR,                 /* vector 28 */
                                     UnimplementedISR,                 /* vector 27 */
                                     UnimplementedISR,                 /* vector 26 */
                                     UnimplementedISR,                 /* vector 25 */
                                     UnimplementedISR,                 /* vector 24 */
                                     UnimplementedISR,                 /* vector 23 */
                                     UnimplementedISR,                 /* vector 22 */
                                     UnimplementedISR,                 /* vector 21 */
                                     UnimplementedISR,                 /* vector 20 */
                                     UnimplementedISR,                 /* vector 19 */
                                     PAI_ISR,                          /* vector 18 */
                                     PAOVI_ISR,                        /* vector 17 */
                                     UnimplementedISR,                 /* vector 16 */
                                     UnimplementedISR,                 /* vector 15 */
                                     UnimplementedISR,                 /* vector 14 */
                                     UnimplementedISR,                 /* vector 13 */
                                     UnimplementedISR,                 /* vector 12 */
                                     UnimplementedISR,                 /* vector 11 */
                                     TC2_ISR,                          /* vector 10 */
                                     TC1_ISR,                          /* vector 09 */
                                     UnimplementedISR,                 /* vector 08 */
                                     RTI_ISR,                          /* vector 07 */
                                     UnimplementedISR,                 /* vector 06 */
                                     UnimplementedISR,                 /* vector 05 */
                                     UnimplementedISR,                 /* vector 04 */
                                     UnimplementedISR,                 /* vector 03 */
                                     UnimplementedISR,                 /* vector 02 */
                                     UnimplementedISR,                 /* vector 01 */
                                     _Startup                          /* Reset vector */
                                 };
