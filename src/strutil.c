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

#include <stdio.h>

#include "strutil.h"

int _scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    int n;
    va_list args;

    va_start(args, fmt);
    n = _vscnprintf(buf, size, fmt, args);
    va_end(args);

    return n;
}

int _vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int n;

    n = vsnprintf(buf, size, fmt, args);

    // if error occured, n <= 0
    return (n <= 0 ? 0 : (n < (int)size ? n : (int)(size -1)));
}
