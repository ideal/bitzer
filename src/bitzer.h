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

#ifndef BZ_CORE_H
#define BZ_CORE_H

#include "config.h"

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <sys/wait.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

typedef intptr_t     bz_int_t;
typedef uintptr_t    bz_uint_t;
typedef intptr_t     bz_flag_t;

typedef struct context_s context_t;

#define BZ_OK      0
#define BZ_ERROR  -1
#define BZ_AGAIN  -2
#define BZ_NOMEM  -3
#define BZ_ABORT  -4

#define OK    BZ_OK
#define ERROR BZ_ERROR

#include "log.h"
#include "util.h"
#include "sighandler.h"
#include "strutil.h"
#include "list.h"
#include "rbtree.h"
#include "task.h"
#include "alloc.h"
#include "conf.h"

#include "context.h"

struct bitzer_s {
    int   log_level;
    char *prefix;
    char *log_file;
    char *conf_file;
    pid_t pid;
    char *pid_file;

    unsigned int log_file_alloc:1;
    unsigned int conf_file_alloc:1;
    unsigned int pid_file_alloc:1;

    char hostname[BZ_MAXHOSTNAMELEN];
    bz_log_t *log;
};

#endif
