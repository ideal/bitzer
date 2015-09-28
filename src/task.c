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
static int task_redirect_io(task_t *task);

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
    task->start_time.tv_sec  = 0;
    task->start_time.tv_usec = 0;
    task->start_count = 0 ;
    task->ctx  = ctx;
    task->path = NULL;
    task->name = NULL;
    task->dir  = NULL;
    task->args = NULL;
    task->args_len   = 0;
    task->args_total = 0;
    task->envp = NULL;
    task->envp_len   = 0;
    task->envp_total = 0;
    task->status = TASK_INIT;
    task->log_path = NULL;

    // NOTE: we have not init list and node

    return OK;
}

static int task_reset(task_t *task)
{
    task->pid    = 0;
    task->status = TASK_FINISHED;
    return OK;
}

static int task_redirect_io(task_t *task)
{
    int fd;
    char *ptr;

    if (!task->log_path) {
        task->log_path = (char *)bz_alloc(strlen(task->ctx->instance->prefix) +
                                          strlen(task->name) + 10, task->ctx->log);
        if (!task->log_path) {
            return ERROR;
        }

        strcpy(task->log_path, task->ctx->instance->prefix);
        strcat(task->log_path, "/log/");
        strcat(task->log_path, task->name);
        strcat(task->log_path, ".log");
    }

    fd = open(task->log_path, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        bz_log_error(task->ctx->log, "open(\"%s\") failed: %s",
                     task->log_path, strerror(errno));
        return ERROR;
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
        ptr = "STDIN";
        goto DUP2FAILED;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        ptr = "STDOUT";
        goto DUP2FAILED;
    }

    if (dup2(fd, STDERR_FILENO) < 0) {
        ptr = "STDERR";
        goto DUP2FAILED;
    }

    close(fd);
    return OK;

DUP2FAILED:
    bz_log_error(task->ctx->log, "redirect task io, dup2(%d, \"%s\") failed: %s",
                 fd, ptr, strerror(errno));
    close(fd);
    return ERROR;
}

int task_run(task_t *task)
{
    int ret;
    pid_t pid;

    task_reset(task);

    if (!task->name || !task->path) {
        bz_log_error(task->ctx->log,
                     "you must set task name and path");
        return ERROR;
    }

    pid = fork();
    switch(pid) {
    case 0:
        task_redirect_io(task);
        if (task->dir && chdir(task->dir) < 0) {
            bz_log_error(task->ctx->log,
                         "change working directory failed, task: %s, error: %s",
                         task->name, strerror(errno));
            _exit(ERROR);
        }
        ret = execv(task->path, task->args);
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
    task->node.key = (rbtree_key_t)pid;
    task->status   = TASK_RUNNING;
    task->start_count++;
    gettimeofday(&task->start_time, NULL);
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
    char **ptr;

    ptr = task->args;
    while (*ptr) {
        free(*ptr++);
    }
    free(task->args);

    if (task->envp) {
        ptr = task->envp;
        while (*ptr) {
            free(*ptr++);
        }
    }
    // ok if envp is NULL
    free(task->envp);

    // NOTE: we can't free task->path here,
    // because it is already been freed in args[0]

    free((char *)task->name);
    free((char *)task->dir);
    free(task->log_path);
    return OK;
}
