/*
 * Copyright (C) 2015 Shang Yuanchun <idealities@gmail.com>
 *
 *
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * FlyingCat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlyingCat. If not, write to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301, USA.
 */

#ifndef BZ_UTIL_H
#define BZ_UTIL_H

#ifdef MAXHOSTNAMELEN
# define BZ_MAXHOSTNAMELEN  MAXHOSTNAMELEN
#else
# define BZ_MAXHOSTNAMELEN  256
#endif

#ifndef __GNUC__
# define __attribute__(x)
# define __FUNCTION__ "<unknown>"
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define BZ_INT32_LEN   (sizeof("-2147483648") - 1)
#define BZ_INT64_LEN   (sizeof("-9223372036854775808") - 1)

#ifndef BZ_ALIGNMENT
#define BZ_ALIGNMENT    sizeof(unsigned long)
#endif

#define BZ_PATHSEP      '/'
#define BZ_LINEFEED     "\n"

#define bz_align(d, a)    (((d) + (a - 1)) & ~(a - 1))
#define bz_align_ptr(p, a) \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#endif
