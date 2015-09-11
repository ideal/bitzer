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

#ifndef BZ_CONTEXT_H
#define BZ_CONTEXT_H

#include "bitzer.h"

typedef void (*signal_callback_t)(void *arg);

typedef struct signal_task_s {
    signal_callback_t callback;
    void *arg;
} signal_task_t;

struct context_s {
    void *conf;
    struct bitzer_s *instance;
    bz_log_t *log;
    signal_task_t signal_task;
    struct list_head tasks_list;
    rbtree_t  tasks_rbtree;
    rbtree_node_t sentinel;
};

extern context_t *context;

extern sig_atomic_t bz_quit;
extern sig_atomic_t bz_terminate;
extern sig_atomic_t bz_reconfigure;
extern sig_atomic_t bz_reopen;
extern sig_atomic_t bz_child;

context_t *context_create(struct bitzer_s *bz);
int context_init(context_t *ctx);
void context_run(context_t *ctx);
void context_set_signal_callback(context_t *ctx, signal_callback_t cb, void *arg);
void context_close(context_t *ctx);

#endif
