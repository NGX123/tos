/*
	@author = code taken from limine bootloader
	@brief 	= library for printing using bootloader services
	@license = BSD 2-Clause "Simplified" License
		Copyright 2019, 2020, 2021 mintsuki and contributors.

		Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

			1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

			2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ed9print.h"
#include <stddef.h>

static const char CONVERSION_TABLE[] = "0123456789abcdef";

void e9_putc(char c) {
    asm volatile ("out dx, al" :: "a" (c), "d" (0xE9) : "memory");
}

void e9_print(const char *msg) {
    for (size_t i = 0; msg[i]; i++) {
        e9_putc(msg[i]);
    }
}

void e9_puts(const char *msg) {
    e9_print(msg);
    e9_putc('\n');
}

static void e9_printhex(size_t num) {
    int i;
    char buf[17];

    if (!num) {
        e9_print("0x0");
        return;
    }

    buf[16] = 0;

    for (i = 15; num; i--) {
        buf[i] = CONVERSION_TABLE[num % 16];
        num /= 16;
    }

    i++;
    e9_print("0x");
    e9_print(&buf[i]);
}

static void e9_printdec(size_t num) {
    int i;
    char buf[21] = {0};

    if (!num) {
        e9_putc('0');
        return;
    }

    for (i = 19; num; i--) {
        buf[i] = (num % 10) + 0x30;
        num = num / 10;
    }

    i++;
    e9_print(buf + i);
}

void e9_printf(const char *format, ...) {
    va_list argp;
    va_start(argp, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'x') {
                e9_printhex(va_arg(argp, size_t));
            } else if (*format == 'd') {
                e9_printdec(va_arg(argp, size_t));
            } else if (*format == 's') {
                e9_print(va_arg(argp, char*));
            }
        } else {
            e9_putc(*format);
        }
        format++;
    }

    e9_putc('\n');
    va_end(argp);
}