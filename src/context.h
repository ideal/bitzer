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

#ifndef BZ_CONTEXT_H
#define BZ_CONTEXT_H

#include "bitzer.h"

struct context_s {
    void *conf;
    struct bitzer_s *instance;
    bz_log_t *log;
};

typedef struct context_s context_t;

extern context_t *context;

extern sig_atomic_t bz_quit;
extern sig_atomic_t bz_terminate;
extern sig_atomic_t bz_reconfigure;
extern sig_atomic_t bz_reopen;

context_t *context_create(struct bitzer_s *bz);
void context_run(context_t *ctx);
void context_close(context_t *ctx);

#endif
