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

static void signal_handler(int signo);

signal_t signals[] = {
    {
      signal_value(SIGNAL_SHUTDOWN),
      signal_name(SIGNAL_SHUTDOWN),
      "quit",
      signal_handler
    },

    {
      signal_value(SIGNAL_TERMINATE),
      signal_name(SIGNAL_TERMINATE),
      "stop",
      signal_handler
    },

    {
      signal_value(SIGNAL_RECONFIGURE),
      signal_name(SIGNAL_RECONFIGURE),
      "reload",
      signal_handler
    },

    {
      signal_value(SIGNAL_REOPEN),
      signal_name(SIGNAL_REOPEN),
      "reopen",
      signal_handler
    },

    {
      SIGINT,
      "SIGINT",
      "stop",
      signal_handler
    },

    { SIGTTIN, "SIGTTIN", "", signal_handler },
    { SIGTTOU, "SIGTTOU", "", signal_handler },
    { SIGPIPE, "SIGPIPE", "", SIG_IGN },

    { 0, NULL, "", NULL }
};

int signal_init(bz_log_t *log)
{
    signal_t *sig;
    struct sigaction sa;

    for (sig = signals; sig->signo != 0; sig++) {
        memset(&sa, 0, sizeof(sa));

        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);

        bz_log_debug(log, BZ_LOG_DEBUG, "sigaction(%s)", sig->signame);
        if (sigaction(sig->signo, &sa, NULL) < 0) {
            bz_log_error(log, "sigaction(%s) failed: %s",
                         sig->signame, strerror(errno));
            return ERROR;
        }
    }

    return OK;
}

void signal_close(bz_log_t *log)
{
    return;
}

void signal_handler(int signo)
{
    const char *action;
    signal_t *sig;

    for (sig = signals; sig->signo != 0; sig++) {
        if (sig->signo == signo) {
            break;
        }
    }

    action = "";
    switch(signo) {
    case signal_value(SIGNAL_SHUTDOWN):
        bz_quit = 1;
        action  = "shutting down";
        break;
    case signal_value(SIGNAL_TERMINATE):
    case SIGINT:
        bz_terminate = 1;
        action  = "exiting";
        break;
    case signal_value(SIGNAL_RECONFIGURE):
        bz_reconfigure = 1;
        action  = "reconfiguring";
        break;
    case signal_value(SIGNAL_REOPEN):
        bz_reopen = 1;
        action  = "reopening logs";
        break;
    default:
        break;
    }

    bz_log(context->log, BZ_LOG_NOTICE, "signal %d(%s) received, %s",
           signo, sig->signame, action);

}

