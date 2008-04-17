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

// LCD模块

#include "includes.h"

/* LCD 的端口引脚定义 */
#define RS PTJ_PTJ1
#define RW PTJ_PTJ0
#define EN PORTK_BIT4
#define DATA PORTB

/* 常用延时, 基于24MHz的总线频率 */
#define ENBIT (0x02)
#define DELAY40US (2000L)
#define DELAY4_1MS (220000L)
#define DELAY100US (5000L)
#define LCDWIDTH (8)

/* 函数定义 */
#define putchar(x) { writeLcd4((x), 1);}


/* 延时函数 */
void lcdDelay(unsigned long constant) {
    volatile unsigned long counter;

    for (counter = constant; counter > 0; counter--);
} // end lcdDelay()


/* Write LCD module in 8-bit mode
 * Inputs:
 *  data: to be written, lower 4 bits are ignored
 *  rs: register select, only bit 0 is significant
 * Handles the shifting into place and the EN pulsing
 * This is only used at the start of the init sequence
 */
void writeLcd8(INT8U data, INT8U rs, INT8U rw) {
    unsigned char temp;

    /* shift upper nibble to data bits in port K */
    RS = rs;
    RW = rw;
    temp = (data);

    /* 产生使能脉冲, 下降延触发 */
    EN = 1;
    DATA = temp;
    EN = 0;
    EN = 1;

    /* 等待完成 */
    lcdDelay(DELAY40US);
} // end writeLcd8()


/* Write LCD module in 4-bit mode
 * Inputs:
 *  data: to be written, 8 bits are significant
 *  rs: register select, only bit 0 is significant
 * Does two consecutive writes, high nibble, then low
 * Handles the shifting into place and the EN pulsing
 * This is can be used at any time (init and display)
 */
void writeLcd4(unsigned char data, unsigned char rs) {
    unsigned char hi, lo;

    hi = ((data & 0xf0) );
    lo = ((data & 0x0f) << 4);
    RS = rs;

    /* do write pulses for upper, then lower nibbles */
    EN = 1;
    DATA = hi;
    EN = 0;
    EN = 1;

    EN = 1;
    DATA = lo;
    EN = 0;
    EN = 1;

    /* allow instruction to complete */
    lcdDelay(DELAY40US);
}

/* 初始化LCD */
void InitLCD(void) {
    /* initialise port */
    DDRB = 0xFF;
    DDRK_BIT4 = 1;
    DDRJ_DDRJ0 = 1;
    DDRJ_DDRJ1 = 1;
    //DDRK = 0xff;

    writeLcd8(0x30, 0 , 0);  // tell it once
    lcdDelay(DELAY4_1MS);
    writeLcd8(0x30, 0 , 0);  // tell it twice
    lcdDelay(DELAY100US);
    writeLcd8(0x30, 0 , 0);  // tell it thrice

    /* last write in 8-bit mode sets bus to 4 bit mode */
    writeLcd8(0x20, 0 , 0);

    /* Now we are in 4 bit mode, write upper/lower nibble */
    writeLcd4(0x28, 0); // last function set: 4-bit mode, 2 lines, 5x7 matrix
    writeLcd4(0x0c, 0); // display on, cursor off, blink off
    writeLcd4(0x01, 0); // display clear
    writeLcd4(0x06, 0); // cursor auto-increment, disable display shift
}

/*******************************************************************************
    itos()
    对itoa的一点延伸,增加了宽度控制和对齐方式, 输入为有符号数
    n: 要转换的数
    base: 基数
    width: 宽度
         < 0 左对齐
         > 0 右对齐
         = 0 代表不限制宽度

*******************************************************************************/
char * itos(INT16S n, INT16S base, INT16S width) {
    register char *p;       /* 字符指针 */
    register INT16S tmp;    /* 即做正负的标志位, 也做长度的计数器 */
    static char buf[18];

    p = &buf[9];

    /* 记下正负, tmp 的第八位为 1 */

    if (n < 0) {
        tmp = 0x81;
        n = -n;
    } else {
        tmp = 0;
    }

    if (n == 0) {
        *--p = '0';
        tmp++;
    } else {
        /* 转换数字 */
        while (n > 0) {
            tmp++;
            *--p = "0123456789ABCDEF"[n % base];
            n /= base;
        }
    }

    /* 添加负号 */
    if ((tmp & 0x80) > 0) {
        *--p = '-';
        tmp &= 0xFF - 0x80;
    }

    if (tmp > _abs(width)) {
        buf[9] = '\0';
        return p;
    }

    /* 改变宽度 */
    if (width >= 0) {
        buf[9] = '\0';
        if (width > 0) {
            tmp = width - tmp;
            while (tmp--) {
                *--p = ' ';
            }
        }
    } else {
        tmp = 9 - (width + tmp);
        buf[tmp] = '\0';
        while (--tmp != 8) {
            buf[tmp] = ' ';
        }
    }

    return p;
}

/*******************************************************************************
    utos()
    对itoa的一点延伸,增加了宽度控制和对齐方式, 输入为无符号数
    n: 要转换的数
    base: 基数
    width: 宽度
         < 0 左对齐
         > 0 右对齐
         = 0 代表不限制宽度

*******************************************************************************/
char * utos(INT16U n, INT16S base, INT16S width) {
    register char *p;       /* 字符指针 */
    register INT16S tmp;    /* 长度的计数器 */
    static char buf[18];

    p = &buf[9];

    tmp = 0;

    if (n == 0) {
        *--p = '0';
        tmp++;
    } else {
        /* 转换数字 */
        while (n > 0) {
            tmp++;
            *--p = "0123456789ABCDEF"[n % base];
            n /= base;
        }
    }

    if (tmp > width) {
        buf[9] = '\0';
        return p;
    }

    /* 改变宽度 */
    if (width >= 0) {
        buf[9] = '\0';
        if (width > 0) {
            tmp = width - tmp;
            while (tmp--) {
                *--p = ' ';
            }
        }
    } else {
        tmp = 9 - (width + tmp);
        buf[tmp] = '\0';
        while (--tmp != 8) {
            buf[tmp] = ' ';
        }
    }

    return p;
}

/******************************************************************************
    printl: print something on LCD
    printf 函数的重写
    主要功能：
    %[[-]n](d|D) 10进制
    %[[-]n](x|X) 16进制
        d和x大小写代表有符号/无符号数(小写有符号)
        n可选代表输出的宽度,前面加负号表示左对齐
        l|L代表16位数,不加则代表8位数
    %[[-]n](s|S) 字符串输出,n可选代表输出的长度
    %c  单个字符输出
******************************************************************************/
void printl(INT16U line, const char * fmt, ...) {
    va_list ap;
    int strlen(const char*);
    const char * s;
    char * p;
    INT16S d, flag, width = 0, justify = 1;
    INT8U instruction;

    /* Set address in LCD module */
    if ( 1 == line) {
        instruction = 0xc0; // write bottom line
    } else {
        instruction = 0x80; // write top line
    }

    writeLcd4( instruction, 0);


    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            putchar(*fmt++);
            continue;
        }
        flag = 1;
        while (flag--) {
            if (*(fmt+1) <= '9' && *(fmt+1) >= '0') {
                width = *++fmt - '0';
            }
            switch (*++fmt) {
            case '-':
                justify = -1;
                width = *++fmt - '0';
                flag++;
                break;
            case 's':
                s = va_arg(ap, const char *);
                width = width - strlen(s);
                if (justify == -1 && width > 0) {
                    while (width--) {
                        putchar(' ');
                    }
                }
                for ( ; *s; s++) {
                    putchar(*s);
                }
                if (justify == 1 && width > 0) {
                    while (width--) {
                        putchar(' ');
                    }
                }
                width = 0;
                justify = 1;
                break;
            case 'd':
                d = va_arg(ap, INT16S);
                p = itos(d, 10, width * justify );
                for (; *p; p++) {
                    putchar(*p);
                }
                width = 0;
                justify = 1;
                break;

            case 'D':
                d = va_arg(ap, INT16U);
                p = utos((INT16U)d, 10, width * justify);
                for (; *p; p++) {
                    putchar(*p);
                }
                width = 0;
                justify = 1;
                break;
            case 'x':
                d = va_arg(ap, INT16S);
                p = itos(d, 16, width * justify);
                for (; *p; p++) {
                    putchar(*p);
                }
                width = 0;
                justify = 1;
                break;
            case 'X':
                d = va_arg(ap, INT16U);
                p = utos((INT16U)d, 16, width * justify);
                for (; *p; p++) {
                    putchar(*p);
                }
                width = 0;
                justify = 1;
                break;
                /* Add other specifiers here... */
            default:
                putchar(*fmt);
                break;
            }
        }
        fmt++;
    }
    //va_end(ap);
}

void ClearLCD(void) {
    writeLcd4(0x01, 0);
}
