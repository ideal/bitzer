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
sig_atomic_t  bz_child;

static int context_sigmask(context_t *ctx, sigset_t *mask);
static int context_event_handler(context_t *ctx);
static task_t *context_find_task(context_t *ctx, pid_t pid);
static int context_start_tasks(context_t *ctx);
static int context_restart_task(context_t *ctx, task_t *task);

context_t *context_create(struct bitzer_s *bz)
{
    context_t *ctx;

    ctx = (context_t *)bz_alloc(sizeof(context_t), bz->log);
    if (ctx == NULL) {
        bz_log_error(bz->log, "create context failed");
        return NULL;
    }

    ctx->instance = bz;
    ctx->log  = bz->log;

    // there is no signal callback currently
    ctx->signal_task.callback = NULL;
    ctx->signal_task.arg      = NULL;

    ctx->conf = NULL;

    // an empty tasks rbtree
    rbtree_init(&ctx->tasks_rbtree, &ctx->sentinel);
    return ctx;
}

int context_init(context_t *ctx)
{
    ctx->conf = conf_create(ctx);
    if (!ctx->conf) {
        return ERROR;
    }

    return conf_load(ctx->conf, ctx->instance->conf_file);
}

void context_set_signal_callback(context_t *ctx, signal_callback_t cb, void *arg)
{
    ctx->signal_task.callback = cb;
    ctx->signal_task.arg      = arg;
}

void context_run(context_t *ctx)
{
    int ret;
    sigset_t mask;

    if (context_sigmask(ctx, &mask) < 0) {
        bz_log_error(ctx->log, "set signal mask failed: %s", strerror(errno));
        return;
    }

    context_start_tasks(ctx);

    while (1) {
        ret = pselect(0, NULL, NULL, NULL, NULL, &ctx->origmask);
        if (ret < 0 && errno == EINTR) {
            context_event_handler(ctx);
            if (ctx->signal_task.callback) {
                ctx->signal_task.callback(ctx->signal_task.arg);
            }
        } else {
            bz_log_error(ctx->log, "unexpected return value from pselect: %d", ret);
        }
    }
}

// TODO: finish tasks
void context_close(context_t *ctx)
{
    struct list_head *pos;

    if (ctx->conf) {
        list_for_each(pos, &ctx->conf->tasks_list) {
            task_close(list_entry(pos, task_t, list));
        }
    }

    conf_close(ctx->conf);
    free(ctx);
}

static int context_sigmask(context_t *ctx, sigset_t *mask)
{
    sigemptyset(mask);
    sigaddset(mask, SIGCHLD);
    sigaddset(mask, SIGINT);
    sigaddset(mask, signal_value(SIGNAL_SHUTDOWN));
    sigaddset(mask, signal_value(SIGNAL_TERMINATE));
    sigaddset(mask, signal_value(SIGNAL_RECONFIGURE));
    sigaddset(mask, signal_value(SIGNAL_REOPEN));

    return sigprocmask(SIG_BLOCK, mask, &ctx->origmask);
}

static int context_event_handler(context_t *ctx)
{
    int status;
    pid_t pid;
    task_t *task;

    if (!bz_child) {
        return OK;
    }

    while (1) {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid == 0) {
            break;
        } else if (pid < 0) {
            bz_log_error(ctx->log, "error at waitpid: %s", strerror(errno));
            break;
        }
        task = context_find_task(ctx, pid);
        if (!task) {
            bz_log_error(ctx->log, "process finished but no related task found, pid: %d", pid);
            continue;
        }
        task_exit_handler(task, status);
        if (context_restart_task(ctx, task) != OK) {
            continue;
        }
    }

    return OK;
}

static task_t *context_find_task(context_t *ctx, pid_t pid)
{
    rbtree_key_t key;
    rbtree_node_t *node, *sentinel;

    key  = pid;
    node = ctx->tasks_rbtree.root;
    sentinel = ctx->tasks_rbtree.sentinel;

    while (node != sentinel) {

        if (key < node->key) {
            node = node->left;
            continue;
        }

        if (key > node->key) {
            node = node->right;
            continue;
        }

        return rbtree_entry(node, task_t, node);
    }

    /* not found */
    return NULL;
}

static int context_start_tasks(context_t *ctx)
{
    task_t *task;
    struct list_head *pos;

    if (list_empty(&ctx->conf->tasks_list)) {
        return OK;
    }

    list_for_each(pos, &ctx->conf->tasks_list) {
        task = list_entry(pos, task_t, list);
        if (task_run(task) == OK) {
            // add to rbtree
            rbtree_insert(&ctx->tasks_rbtree, &task->node);
            bz_log(ctx->log, BZ_LOG_INFO, "starting task succeed, name: %s, pid: %d", task->name, task->pid);
        } else {
            bz_log_error(ctx->log, "starting task failed, name: %s", task->name);
        }
    }
    return OK;
}

static int context_restart_task(context_t *ctx, task_t *task)
{
    // first remove from rbtree
    rbtree_delete(&ctx->tasks_rbtree, &task->node);

    if (task_run(task) == OK) {
        // add to rbtree again
        rbtree_insert(&ctx->tasks_rbtree, &task->node);
        bz_log(ctx->log, BZ_LOG_INFO, "restarting task succeed, name: %s, pid: %d", task->name, task->pid);
        return OK;
    }

    bz_log_error(ctx->log, "restarting task failed, name: %s", task->name);
    return ERROR;
}
