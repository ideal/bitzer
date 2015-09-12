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

static int task_reset(task_t *task);

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
    task->status = TASK_INIT;

    // NOTE: we have not init list and node

    return OK;
}

static int task_reset(task_t *task)
{
    task->pid    = 0;
    task->status = TASK_FINISHED;
    return OK;
}

int task_run(task_t *task)
{
    int ret;
    pid_t pid;

    task_reset(task);

    pid = fork();
    switch(pid) {
    case 0:
        ret = execv(task->file, task->args);
        if (ret < 0) {
            bz_log_error(task->ctx->log,
                         "call execv failed, task: %s, error: %s",
                         task->name, strerror(errno));
            _exit(ERROR);
        }
        break;
    case -1:
        bz_log_error(task->ctx->log,
                     "fork process failed, task: %s, error: %s",
                     task->name, strerror(errno));
        return ERROR;
    }

    task->pid    = pid;
    task->status = TASK_RUNNING;
    return OK;
}

int task_exit_handler(task_t *task, int status)
{
    if (WIFEXITED(status)) {
        bz_log(task->ctx->log, BZ_LOG_INFO,
               "task(%s) finished, return value: %d",
               task->name, WEXITSTATUS(status));
        return OK;
    }

    bz_log(task->ctx->log, BZ_LOG_INFO,
           "task(%s) finished, by signal: %d",
           task->name, WTERMSIG(status));
    return OK;
}

int task_close(task_t *task)
{
    return OK;
}
