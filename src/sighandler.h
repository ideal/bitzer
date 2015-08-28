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

#ifndef BZ_SIGNAL_H
#define BZ_SIGNAL_H

#include <signal.h>

#define signal_name_help(n)  "SIG" #n
#define signal_name(n)       signal_name_help(n)

#define signal_value_help(n) SIG##n
#define signal_value(n)      signal_value_help(n)

#define SIGNAL_SHUTDOWN      QUIT
#define SIGNAL_TERMINATE     TERM
#define SIGNAL_RECONFIGURE   HUP
#define SIGNAL_REOPEN        USR1

typedef struct {
    int   signo;
    char *signame;
    char *name;
    void (*handler)(int signo);
} signal_t;

int  signal_init(bz_log_t *log);
void signal_close(bz_log_t *log);

#endif
