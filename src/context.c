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

#include "context.h"

context_t *context;

sig_atomic_t  bz_quit;
sig_atomic_t  bz_terminate;
sig_atomic_t  bz_reconfigure;
sig_atomic_t  bz_reopen;

context_t *context_create(struct bitzer_s *bz)
{
    context_t *ctx;

    ctx = (context_t *)malloc(sizeof(context_t));
    if (ctx == NULL) {
        bz_log_error(bz->log, "create context failed");
        return NULL;
    }

    ctx->instance = bz;
    ctx->log  = bz->log;

    return ctx;
}

void context_run(context_t *ctx)
{
    for (;;) {
        break;
    }
}

void context_close(context_t *ctx)
{
    free(ctx);
}

