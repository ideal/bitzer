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

#ifndef BZ_TASK_H
#define BZ_TASK_H

#include "bitzer.h"

typedef struct task_s {
    pid_t  pid;
    time_t start_time;
    size_t start_count;
    context_t *ctx;
    const char *file;
    const char *name;
    const char * const *args;
    struct list_head list;
    rbtree_node_t node;
} task_t;

task_t *task_create(context_t *ctx);
int task_init(task_t *task, context_t *ctx);
int task_run(task_t *task);
int task_close(task_t *task);

#endif
