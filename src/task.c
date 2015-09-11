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

#include "bitzer.h"

task_t *task_create(context_t *ctx)
{
    task_t *task;

    task = (task_t *)bz_alloc(sizeof(task_t), ctx->log);
    if (task == NULL) {
        bz_log_error(ctx->log, "create task failed");
        return NULL;
    }

    task_init(task, ctx);

    return task;
}

int task_init(task_t *task, context_t *ctx)
{
    task->pid = 0;
    task->start_time  = 0;
    task->start_count = 0 ;
    task->ctx  = ctx;
    task->file = NULL;
    task->name = NULL;
    task->args = NULL;

    // NOTE: we have not init list and node

    return OK;
}

int task_run(task_t *task)
{
    return OK;
}

int task_close(task_t *task)
{
    return OK;
}
