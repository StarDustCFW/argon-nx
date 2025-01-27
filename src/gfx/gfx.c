/*
 * Copyright (c) 2018 naehrwert
 * Copyright (C) 2018 CTCaer
 * Copyright (C) 2018 Guillem96
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include "gfx/gfx.h"
#include "gfx/di.h"

#include "menu/tools/fs_utils.h"
#include "utils/util.h"
#include "mem/heap.h"
#include <string.h>

#define TRANSPARENT_COLOR 0xFF1D1919

gfx_ctxt_t g_gfx_ctxt;
gfx_con_t g_gfx_con;

static const u8 _gfx_font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Char 032 ( )
    0x00, 0x30, 0x30, 0x18, 0x18, 0x00, 0x0C, 0x00, // Char 033 (!)
    0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, // Char 034 (")
    0x00, 0x66, 0x66, 0xFF, 0x66, 0xFF, 0x66, 0x66, // Char 035 (#)
    0x00, 0x18, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x18, // Char 036 ($)
    0x00, 0x46, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x62, // Char 037 (%)
    0x00, 0x3C, 0x66, 0x3C, 0x1C, 0xE6, 0x66, 0xFC, // Char 038 (&)
    0x00, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, // Char 039 (')
    0x00, 0x30, 0x18, 0x0C, 0x0C, 0x18, 0x30, 0x00, // Char 040 (()
    0x00, 0x0C, 0x18, 0x30, 0x30, 0x18, 0x0C, 0x00, // Char 041 ())
    0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, // Char 042 (*)
    0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, // Char 043 (+)
    0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00, // Char 044 (,)
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, // Char 045 (-)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, // Char 046 (.)
    0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, // Char 047 (/)
    0x00, 0x3C, 0x66, 0x76, 0x6E, 0x66, 0x3C, 0x00, // Char 048 (0)
    0x00, 0x18, 0x1C, 0x18, 0x18, 0x18, 0x7E, 0x00, // Char 049 (1)
    0x00, 0x3C, 0x62, 0x30, 0x0C, 0x06, 0x7E, 0x00, // Char 050 (2)
    0x00, 0x3C, 0x62, 0x38, 0x60, 0x66, 0x3C, 0x00, // Char 051 (3)
    0x00, 0x6C, 0x6C, 0x66, 0xFE, 0x60, 0x60, 0x00, // Char 052 (4)
    0x00, 0x7E, 0x06, 0x7E, 0x60, 0x66, 0x3C, 0x00, // Char 053 (5)
    0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x3C, 0x00, // Char 054 (6)
    0x00, 0x7E, 0x30, 0x30, 0x18, 0x18, 0x18, 0x00, // Char 055 (7)
    0x00, 0x3C, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00, // Char 056 (8)
    0x00, 0x3C, 0x66, 0x7C, 0x60, 0x66, 0x3C, 0x00, // Char 057 (9)
    0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, // Char 058 (:)
    0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x0C, 0x00, // Char 059 (;)
    0x00, 0x70, 0x1C, 0x06, 0x06, 0x1C, 0x70, 0x00, // Char 060 (<)
    0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00, // Char 061 (=)
    0x00, 0x0E, 0x38, 0x60, 0x60, 0x38, 0x0E, 0x00, // Char 062 (>)
    0x00, 0x3C, 0x66, 0x30, 0x18, 0x00, 0x18, 0x00, // Char 063 (?)
    0x00, 0x3C, 0x66, 0x76, 0x76, 0x06, 0x46, 0x3C, // Char 064 (@)
    0x00, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, // Char 065 (A)
    0x00, 0x3E, 0x66, 0x3E, 0x66, 0x66, 0x3E, 0x00, // Char 066 (B)
    0x00, 0x3C, 0x66, 0x06, 0x06, 0x66, 0x3C, 0x00, // Char 067 (C)
    0x00, 0x1E, 0x36, 0x66, 0x66, 0x36, 0x1E, 0x00, // Char 068 (D)
    0x00, 0x7E, 0x06, 0x1E, 0x06, 0x06, 0x7E, 0x00, // Char 069 (E)
    0x00, 0x3E, 0x06, 0x1E, 0x06, 0x06, 0x06, 0x00, // Char 070 (F)
    0x00, 0x3C, 0x66, 0x06, 0x76, 0x66, 0x3C, 0x00, // Char 071 (G)
    0x00, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, // Char 072 (H)
    0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // Char 073 (I)
    0x00, 0x78, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, // Char 074 (J)
    0x00, 0x66, 0x36, 0x1E, 0x1E, 0x36, 0x66, 0x00, // Char 075 (K)
    0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00, // Char 076 (L)
    0x00, 0x46, 0x6E, 0x7E, 0x56, 0x46, 0x46, 0x00, // Char 077 (M)
    0x00, 0x66, 0x6E, 0x7E, 0x76, 0x66, 0x66, 0x00, // Char 078 (N)
    0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 079 (O)
    0x00, 0x3E, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x00, // Char 080 (P)
    0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x70, 0x00, // Char 081 (Q)
    0x00, 0x3E, 0x66, 0x3E, 0x1E, 0x36, 0x66, 0x00, // Char 082 (R)
    0x00, 0x3C, 0x66, 0x0C, 0x30, 0x66, 0x3C, 0x00, // Char 083 (S)
    0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // Char 084 (T)
    0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 085 (U)
    0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // Char 086 (V)
    0x00, 0x46, 0x46, 0x56, 0x7E, 0x6E, 0x46, 0x00, // Char 087 (W)
    0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00, // Char 088 (X)
    0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00, // Char 089 (Y)
    0x00, 0x7E, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00, // Char 090 (Z)
    0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, // Char 091 ([)
    0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, // Char 092 (\)
    0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, // Char 093 (])
    0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, // Char 094 (^)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // Char 095 (_)
    0x00, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, // Char 096 (`)
    0x00, 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x7C, 0x00, // Char 097 (a)
    0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00, // Char 098 (b)
    0x00, 0x00, 0x3C, 0x06, 0x06, 0x06, 0x3C, 0x00, // Char 099 (c)
    0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x7C, 0x00, // Char 100 (d)
    0x00, 0x00, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00, // Char 101 (e)
    0x00, 0x38, 0x0C, 0x3E, 0x0C, 0x0C, 0x0C, 0x00, // Char 102 (f)
    0x00, 0x00, 0x7C, 0x66, 0x7C, 0x40, 0x3C, 0x00, // Char 103 (g)
    0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x00, // Char 104 (h)
    0x00, 0x18, 0x00, 0x1C, 0x18, 0x18, 0x3C, 0x00, // Char 105 (i)
    0x00, 0x30, 0x00, 0x30, 0x30, 0x30, 0x1E, 0x00, // Char 106 (j)
    0x00, 0x06, 0x06, 0x36, 0x1E, 0x36, 0x66, 0x00, // Char 107 (k)
    0x00, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // Char 108 (l)
    0x00, 0x00, 0x66, 0xFE, 0xFE, 0xD6, 0xC6, 0x00, // Char 109 (m)
    0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x00, // Char 110 (n)
    0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 111 (o)
    0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x00, // Char 112 (p)
    0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x00, // Char 113 (q)
    0x00, 0x00, 0x3E, 0x66, 0x06, 0x06, 0x06, 0x00, // Char 114 (r)
    0x00, 0x00, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x00, // Char 115 (s)
    0x00, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x70, 0x00, // Char 116 (t)
    0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, // Char 117 (u)
    0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // Char 118 (v)
    0x00, 0x00, 0xC6, 0xD6, 0xFE, 0x7C, 0x6C, 0x00, // Char 119 (w)
    0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00, // Char 120 (x)
    0x00, 0x00, 0x66, 0x66, 0x7C, 0x60, 0x3C, 0x00, // Char 121 (y)
    0x00, 0x00, 0x7E, 0x30, 0x18, 0x0C, 0x7E, 0x00, // Char 122 (z)
    0x00, 0x18, 0x08, 0x08, 0x04, 0x08, 0x08, 0x18, // Char 123 ({)
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, // Char 124 (|)
    0x00, 0x0C, 0x08, 0x08, 0x10, 0x08, 0x08, 0x0C, // Char 125 (})
    0x00, 0x00, 0x00, 0x4C, 0x32, 0x00, 0x00, 0x00  // Char 126 (~)
};

void gfx_init_ctxt(gfx_ctxt_t *ctxt, u32 *fb, u32 width, u32 height, u32 stride)
{
    ctxt->fb = fb;
    ctxt->width = width;
    ctxt->height = height;
    ctxt->stride = stride;
    ctxt->next = fb + ctxt->width * ctxt->stride * 4;
    set_active_framebuffer(fb);
}

void gfx_end_ctxt(gfx_ctxt_t *ctxt)
{
    gfx_clear_buffer(ctxt);
    free(ctxt->next);
    free(ctxt);
}

void gfx_clear_buffer(gfx_ctxt_t *ctxt)
{
    memset(ctxt->next, 0, ctxt->width * ctxt->stride * 4);
}
void gfx_swap_buffer(gfx_ctxt_t *ctxt)
{
    u32* tmp = ctxt->fb;
    ctxt->fb = ctxt->next;
    ctxt->next = tmp;
    set_active_framebuffer(ctxt->fb);
    gfx_clear_buffer(ctxt);
}

void gfx_clear_grey(gfx_ctxt_t *ctxt, u8 color)
{
	memset(ctxt->next, color, ctxt->width * ctxt->stride * 4);
}

void gfx_clear_color(gfx_ctxt_t *ctxt, u32 color)
{
    memset(ctxt->next, color, ctxt->width * ctxt->stride * 4);
}

void gfx_clear_partial_grey(gfx_ctxt_t *ctxt, u8 color, u32 pos_x, u32 height)
{
    memset(ctxt->next + pos_x * ctxt->stride, color, height * 4 * ctxt->stride);
}

void gfx_con_init(gfx_con_t *con, gfx_ctxt_t *ctxt)
{
    con->gfx_ctxt = ctxt;
    con->scale = 2;
    con->x = 0;
    con->y = 0;
    con->savedx = 0;
    con->savedy = 0;
    con->fgcol = 0xFFCCCCCC;
    con->fillbg = 0;
    con->bgcol = 0xFF1B1B1B;
    con->mute = 0;
}

void gfx_con_setcol(gfx_con_t *con, u32 fgcol, int fillbg, u32 bgcol)
{
    con->fgcol = fgcol;
    con->fillbg = fillbg;
    con->bgcol = bgcol;
}

void gfx_con_getpos(gfx_con_t *con, u32 *x, u32 *y)
{
    *x = con->x;
    *y = con->y;
}

void gfx_con_setpos(gfx_con_t *con, u32 x, u32 y)
{
    con->x = x;
    con->y = y;
}

void gfx_putc(gfx_con_t *con, char c)
{
    if (c >= 32 && c <= 126)
    {
        u8 *cbuf = (u8 *)&_gfx_font[CHAR_WIDTH * (c - 32)];
        for (u32 i = 0; i < CHAR_HEIGHT * con->scale; i += con->scale)
        {
            u8 v = *cbuf++;
            
            for (u32 j = 0; j < CHAR_WIDTH; j++)
            {
                for(u32 z = 0; z < con->scale; z++)
                {
                   if (v & 1)
                    {
                        for(u32 k = 0; k < con->scale; k++)
                        {
                            gfx_set_pixel(con->gfx_ctxt, con->x + k + j * con->scale, con->y + i + z, con->fgcol);   
                        }
                    }
                    else if (con->fillbg)
                    {
                        for(u32 k = 0; k < con->scale; k++)
                        {
                            gfx_set_pixel(con->gfx_ctxt, con->x + k + j * con->scale, con->y + i + z, con->bgcol);
                        }
                    }
                }
                v >>= 1;
            }            
        }
        con->x += CHAR_WIDTH * con->scale;
    }
    else if (c == '\n')
    {
        con->x = 0;
        con->y += CHAR_HEIGHT * con->scale;
        if (con->y > con->gfx_ctxt->height - CHAR_HEIGHT)
            con->y = 0;
    }
}

void gfx_puts(gfx_con_t *con, const char *s)
{
    if (!s || con->mute)
        return;

    for (; *s; s++)
        gfx_putc(con, *s);
}

static void _gfx_putn(gfx_con_t *con, u32 v, int base, char fill, int fcnt)
{
    char buf[65];
    static const char digits[] = "0123456789ABCDEFghijklmnopqrstuvwxyz";
    char *p;
    int c = fcnt;

    if (base > 36)
        return;

    p = buf + 64;
    *p = 0;
    do
    {
        c--;
        *--p = digits[v % base];
        v /= base;
    } while (v);

    if (fill != 0)
    {
        while (c > 0)
        {
            *--p = fill;
            c--;
        }
    }

    gfx_puts(con, p);
}

void gfx_put_small_sep(gfx_con_t *con)
{
    u8 prevFontSize = con->scale;
    con->scale = 1;
    gfx_putc(con, '\n');
    con->scale = prevFontSize;
}

void gfx_put_big_sep(gfx_con_t *con)
{
    u8 prevFontSize = con->scale;
    con->scale = 2;
    gfx_putc(con, '\n');
    con->scale = prevFontSize;
}

void gfx_printf(gfx_con_t *con, const char *fmt, ...)
{
    if (con->mute)
        return;

    va_list ap;
    int fill, fcnt;

    va_start(ap, fmt);
    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            fill = 0;
            fcnt = 0;
            if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ')
            {
                fcnt = *fmt;
                fmt++;
                if (*fmt >= '0' && *fmt <= '9')
                {
                    fill = fcnt;
                    fcnt = *fmt - '0';
                    fmt++;
                }
                else
                {
                    fill = ' ';
                    fcnt -= '0';
                }
            }
            switch (*fmt)
            {
            case 'c':
                gfx_putc(con, va_arg(ap, u32));
                break;
            case 's':
                gfx_puts(con, va_arg(ap, char *));
                break;
            case 'd':
                _gfx_putn(con, va_arg(ap, u32), 10, fill, fcnt);
                break;
            case 'p':
            case 'P':
            case 'x':
            case 'X':
                _gfx_putn(con, va_arg(ap, u32), 16, fill, fcnt);
                break;
            case 'k':
                con->fgcol = va_arg(ap, u32);
                break;
            case 'K':
                con->bgcol = va_arg(ap, u32);
                con->fillbg = 1;
                break;
            case '%':
                gfx_putc(con, '%');
                break;
            case '\0':
                goto out;
            default:
                gfx_putc(con, '%');
                gfx_putc(con, *fmt);
                break;
            }
        }
        else
            gfx_putc(con, *fmt);
        fmt++;
    }

out:
    va_end(ap);
}

void gfx_hexdump(gfx_con_t *con, u32 base, const u8 *buf, u32 len)
{
    if (con->mute)
        return;

    u8 prevFontSize = con->scale;
    con->scale = 1;
    for (u32 i = 0; i < len; i++)
    {
        if (i % 0x10 == 0)
        {
            if (i != 0)
            {
                gfx_puts(con, "| ");
                for (u32 j = 0; j < 0x10; j++)
                {
                    u8 c = buf[i - 0x10 + j];
                    if (c >= 32 && c <= 126)
                        gfx_putc(con, c);
                    else
                        gfx_putc(con, '.');
                }
                gfx_putc(con, '\n');
            }
            gfx_printf(con, "%08x: ", base + i);
        }
        gfx_printf(con, "%02x ", buf[i]);
        if (i == len - 1)
        {
            int ln = len % 0x10 != 0;
            u32 k = 0x10 - 1;
            if (ln)
            {
                k = (len & 0xF) - 1;
                for (u32 j = 0; j < 0x10 - k; j++)
                    gfx_puts(con, "   ");
            }
            gfx_puts(con, "| ");
            for (u32 j = 0; j < (ln ? k : k + 1); j++)
            {
                u8 c = buf[i - k + j];
                if (c >= 32 && c <= 126)
                    gfx_putc(con, c);
                else
                    gfx_putc(con, '.');
            }
            gfx_putc(con, '\n');
        }
    }
    gfx_putc(con, '\n');
    con->scale = prevFontSize;
}

static int abs(int x)
{
    if (x < 0)
        return -x;
    return x;
}

void gfx_set_pixel(gfx_ctxt_t *ctxt, u32 x, u32 y, u32 color)
{
    ctxt->next[y + (ctxt->width - x) * ctxt->stride] = color;
}

void gfx_line(gfx_ctxt_t *ctxt, int x0, int y0, int x1, int y1, u32 color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1)
    {
        gfx_set_pixel(ctxt, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void gfx_set_rect_grey(gfx_ctxt_t *ctxt, const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
    u32 pos = 0;
    for (u32 y = pos_y; y < (pos_y + size_y); y++)
    {
        for (u32 x = pos_x; x < (pos_x + size_x); x++)
        {
            memset(&ctxt->next[x + y * ctxt->stride], buf[pos], 4);
            pos++;
        }
    }
}

void gfx_set_rect_rgb(gfx_ctxt_t *ctxt, const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
    u32 pos = 0;
    for (u32 y = pos_y; y < (pos_y + size_y); y++)
    {
        for (u32 x = pos_x; x < (pos_x + size_x); x++)
        {
            ctxt->next[x + y * ctxt->stride] = buf[pos + 2] | (buf[pos + 1] << 8) | (buf[pos] << 16);
            pos += 3;
        }
    }
}

void gfx_set_rect_argb(gfx_ctxt_t *ctxt, const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
    u32 *ptr = (u32 *)buf;
    for (u32 y = pos_y; y < (pos_y + size_y); y++)
        for (u32 x = pos_x; x < (pos_x + size_x); x++)
            ctxt->next[x + y * ctxt->stride] = *ptr++;
}

void gfx_render_bmp_argb(gfx_ctxt_t *ctxt, const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
    gfx_render_bmp_argb_transparent(ctxt, buf, size_x, size_y, pos_x, pos_y, TRANSPARENT_COLOR);
}

void gfx_render_bmp_argb_transparent(gfx_ctxt_t *ctxt, const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y, u32 transparent_color)
{
    for (u32 y = pos_y; y < (pos_y + size_y); y++)
    {
        for (u32 x = pos_x; x < (pos_x + size_x); x++) 
        {
            u32 render_color = buf[(size_y + pos_y - 1 - y) * size_x + x - pos_x];
        
            if (render_color != transparent_color)
                gfx_set_pixel(ctxt, x, y, render_color);
        }
    }
}


void gfx_render_bmp_arg_file(gfx_ctxt_t *ctxt, char *path, u32 x, u32 y, u32 width, u32 height)
{
    u8 *bitmap = (u8 *)sd_file_read(path);
    gfx_render_bmp_arg_bitmap(ctxt, bitmap, x, y, width, height);
}

void gfx_render_bmp_arg_bitmap_transparent(gfx_ctxt_t *ctxt, u8 *bitmap, u32 x, u32 y, u32 width, u32 height, u32 transparent_color)
{
    bmp_data_t bmp_data;
    u8 *image = NULL;
    bool image_found = false;

    if (bitmap != NULL)
    {
        // Get values manually to avoid unaligned access.
        bmp_data.size = bitmap[2] | bitmap[3] << 8 |
                        bitmap[4] << 16 | bitmap[5] << 24;
        bmp_data.offset = bitmap[10] | bitmap[11] << 8 |
                          bitmap[12] << 16 | bitmap[13] << 24;
        bmp_data.size_x = bitmap[18] | bitmap[19] << 8 |
                          bitmap[20] << 16 | bitmap[21] << 24;
        bmp_data.size_y = bitmap[22] | bitmap[23] << 8 |
                          bitmap[24] << 16 | bitmap[25] << 24;
        // Sanity check.
        if (bitmap[0] == 'B' &&
            bitmap[1] == 'M' &&
            bitmap[28] == 32 && //
            bmp_data.size_x <= width &&
            bmp_data.size_y <= height)
        {
            if ((bmp_data.size - bmp_data.offset) <= 0x400000)
            {
                // Avoid unaligned access from BM 2-byte MAGIC and remove header.
                image = (u8 *)malloc(0x400000);
                memcpy(image, bitmap + bmp_data.offset, bmp_data.size - bmp_data.offset);
                bmp_data.pos_x = (width - bmp_data.size_x) >> 1;
                bmp_data.pos_y = (height - bmp_data.size_y) >> 1;

                // Get background color from 1st pixel.
				if (bmp_data.size_x < width || bmp_data.size_y < height)
                    gfx_clear_color(ctxt, *(u32 *)image);
                    
                image_found = true;
            }
        }
    }
    if (image_found)
    {
        gfx_render_bmp_argb_transparent(ctxt, (u32 *)image, bmp_data.size_x, bmp_data.size_y,
                            bmp_data.pos_x + x, bmp_data.pos_y + y, transparent_color);
    }
    free(image);
}


void gfx_render_bmp_arg_bitmap(gfx_ctxt_t *ctxt, u8 *bitmap, u32 x, u32 y, u32 width, u32 height)
{
    gfx_render_bmp_arg_bitmap_transparent(ctxt, bitmap, x, y, width, height, TRANSPARENT_COLOR);
}

void gfx_render_splash(gfx_ctxt_t *ctxt, u8 *bitmap)
{
    bmp_data_t bmp_data;
    u8 *image = NULL;
    bool image_found = false;

    if (bitmap != NULL)
    {
        // Get values manually to avoid unaligned access.
        bmp_data.size = bitmap[2] | bitmap[3] << 8 |
                        bitmap[4] << 16 | bitmap[5] << 24;
        bmp_data.offset = bitmap[10] | bitmap[11] << 8 |
                          bitmap[12] << 16 | bitmap[13] << 24;
        bmp_data.size_x = bitmap[18] | bitmap[19] << 8 |
                          bitmap[20] << 16 | bitmap[21] << 24;
        bmp_data.size_y = bitmap[22] | bitmap[23] << 8 |
                          bitmap[24] << 16 | bitmap[25] << 24;
        // Sanity check.
        if (bitmap[0] == 'B' &&
            bitmap[1] == 'M' &&
            bitmap[28] == 32 && //
            bmp_data.size_x <= ctxt->height &&
            bmp_data.size_y <= ctxt->width)
        {
            if ((bmp_data.size - bmp_data.offset) <= 0x400000)
            {
                // Avoid unaligned access from BM 2-byte MAGIC and remove header.
                image = (u8 *)malloc(0x400000);
                memcpy(image, bitmap + bmp_data.offset, bmp_data.size - bmp_data.offset);
                bmp_data.pos_x = (ctxt->height - bmp_data.size_x) >> 1;
                bmp_data.pos_y = (ctxt->width - bmp_data.size_y) >> 1;

                // Get background color from 1st pixel.
				if (bmp_data.size_x < ctxt->height || bmp_data.size_y < ctxt->width)
                    gfx_clear_color(ctxt, *(u32 *)image);
                    
                image_found = true;
            }
        }
    }
    if (image_found)
    {
        u32* buf = (u32*)image;
        for (u32 y = bmp_data.pos_y; y < (bmp_data.pos_y + bmp_data.size_y); y++)
        {
            for (u32 x =  bmp_data.pos_x; x < (bmp_data.pos_x + bmp_data.size_x); x++)
	            ctxt->next[x + y * ctxt->stride] = buf[(bmp_data.size_y + bmp_data.pos_y - 1 - y ) * bmp_data.size_x + x - bmp_data.pos_x];        
        }

    }
    free(image);
}
