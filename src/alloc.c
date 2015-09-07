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

#include "alloc.h"

void *bz_alloc(size_t size, bz_log_t *log)
{
    void *p = malloc(size);

    if (p == NULL) {
        bz_log_error(log, "malloc(%zu) failed", size);
    }

    bz_log_debug(log, BZ_LOG_DEBUG, "malloc: %zu at: %p", size, p);
    return p;
}

void *bz_calloc(size_t size, bz_log_t *log)
{
    void *p = bz_alloc(size, log);

    return (p != NULL ? memset(p, 0, size) : p);
}

