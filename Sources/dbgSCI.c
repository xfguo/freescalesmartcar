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

// Debug Modle : Serial Port

#include "includes.h"


void InitSCI0(void) {
    SCI0BDL = (unsigned char)((48000000UL /* OSC freq */ / 2) / 115200 /* baud rate */ / 16 /*factor*/);
    SCI0CR2 = 0x2C;
}

void WriteSCI0String(const char *text) {
    while (*text != '\0') {
        while (!(SCI0SR1 & 0x80));  /* wait for output buffer empty */
        SCI0DRL = *text++;
    }
}

void WriteSCI0Char(unsigned char ch) {
    while (!(SCI0SR1 & 0x80));
    SCI0DRL = ch;
}

/******************************************************************************
    prints: print something to Serial Port
    printf 函数的重写
    主要功能：
    %[[-]n](d|D) 10进制
    %[[-]n](x|X) 16进制
        d和x大小写代表有符号/无符号数(小写有符号)
        n可选代表输出的宽度,前面加负号表示左对齐
    %[[-]n](s|S) 字符串输出,n可选代表输出的长度
    %c  单个字符输出
******************************************************************************/
void prints(const char * fmt, ...) {
    va_list ap;
    int strlen(const char*);
    const char * s;
    char * p;
    INT16S d, flag, width = 0, justify = 1;

    va_start(ap, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            WriteSCI0Char(*fmt++);
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
                        WriteSCI0Char(' ');
                    }
                }
                for ( ; *s; s++) {
                    WriteSCI0Char(*s);
                }
                if (justify == 1 && width > 0) {
                    while (width--) {
                        WriteSCI0Char(' ');
                    }
                }
                width = 0;
                justify = 1;
                break;
            case 'd':
                d = va_arg(ap, INT16S);
                p = itos(d, 10, width * justify );
                for (; *p; p++) {
                    WriteSCI0Char(*p);
                }
                width = 0;
                justify = 1;
                break;

            case 'D':
                d = va_arg(ap, INT16U);
                p = utos((INT16U)d, 10, width * justify);
                for (; *p; p++) {
                    WriteSCI0Char(*p);
                }
                width = 0;
                justify = 1;
                break;
            case 'x':
                d = va_arg(ap, INT16S);
                p = itos(d, 16, width * justify);
                for (; *p; p++) {
                    WriteSCI0Char(*p);
                }
                width = 0;
                justify = 1;
                break;
            case 'X':
                d = va_arg(ap, INT16U);
                p = utos((INT16U)d, 16, width * justify);
                for (; *p; p++) {
                    WriteSCI0Char(*p);
                }
                width = 0;
                justify = 1;
                break;
                /* Add other specifiers here... */
            default:
                WriteSCI0Char(*fmt);
                break;
            }
        }
        fmt++;
    }
    //va_end(ap);
}

