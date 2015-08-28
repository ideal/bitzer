/*
 * Copyright (C) 2015 Shang Yuanchun <idealities@gmail.com>
 *
 *
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * Bitzer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bitzer. If not, write to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301, USA.
 */

#ifndef BZ_STRING_H
#define BZ_STRING_H

#include <sys/types.h>
#include <stdarg.h>

struct string {
    u_char *data;
    size_t  len;
};

#define string(_str) { _str, sizeof(_str) - 1 }
#define null_string(_str) { NULL, 0 }

#define bz_scnprintf(_buf, _size, ...)          \
        _scnprintf((char *)(_buf), (size_t)(_size), __VA_ARGS__)
    
#define bz_vscnprintf(_buf, _size, _fmt, _args)   \
        _vscnprintf((char *)(_buf), (size_t)(_size), _fmt, _args)

int _scnprintf(char *buf, size_t size, const char *fmt, ...);
int _vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

#endif
