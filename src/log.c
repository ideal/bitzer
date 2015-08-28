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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "strutil.h"

#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
# include <execinfo.h>
#endif

static const char *loglevel_desc[] = {
    "emerge",
    "alert",
    "critical",
    "error",
    "warning",
    "notice",
    "info",
    "debug",
    "verb",
};

#define BZ_LOG_MAX_LEVEL BZ_LOG_VERB
#define MAX_LEVEL_DESC_LEN  (sizeof("critical") - 1)

static inline const char *bz_basename(const char *path)
{
    const char *ptr = strrchr(path, '/');

    return (ptr ? ptr + 1 : path);
}

bz_log_t *bz_log_create(int level, const char *filename)
{
    bz_log_t *log = (bz_log_t *)malloc(sizeof(bz_log_t));
    if (log == NULL) {
        return NULL;
    }

    log->log_level = min(BZ_LOG_VERB, max(BZ_LOG_EMERG, level));
    log->file = (char *)filename;
    log->nerr = 0;
    if (!filename || !strlen(filename)) {
        log->fd = STDERR_FILENO;
        return log;
    }

    log->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (log->fd < 0) {
        bz_log_stderr("open log file '%s' failed: %s", filename,
                      strerror(errno));
        free(log);
        return NULL;
    }
    return log;
}

void bz_log_close(bz_log_t *log)
{
    if (!log) {
        return;
    }

    if (log->fd > 0 && log->fd != STDERR_FILENO) {
        close(log->fd);
    }

    free(log);
}

void bz_log_reopen(bz_log_t *log)
{
    if (!log || log->fd == STDERR_FILENO) {
        return;
    }

    if (log->fd > 0) {
        close(log->fd);
    }
    log->fd = open(log->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (log->fd < 0) {
        bz_log_stderr("reopen log file '%s' failed: %s", log->file,
                      strerror(errno));
    }
}

void _log(bz_log_t *log, const char *file, int line, int level, const char *fmt, ...)
{
    int     len, size, errno_save;
    char    buf[BZ_MAX_ERR_STR];
    va_list args;
    struct  timeval tv;
    struct  tm *tm;

    if (!log || log->fd < 0) {
        return;
    }

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    errno_save = errno;
    len  = 0;
    size = BZ_MAX_ERR_STR;
    len += bz_scnprintf(buf + len, size - len, "[%02d-%02d %02d:%02d:%02d.%ld] "
                                               "[%*s] [%-12s:%-4d] ",
                        tm->tm_mon,  tm->tm_mday,
                        tm->tm_hour, tm->tm_min, tm->tm_sec,
                        tv.tv_usec,
                        MAX_LEVEL_DESC_LEN,
                        loglevel_desc[level],
                        bz_basename(file), line);

    va_start(args, fmt);
    len += bz_vscnprintf(buf + len, size - len, fmt, args);
    va_end(args);

    // we do not need '\0'
    buf[len++] = '\n';

    if (write(log->fd, buf, len) < 0) {
        log->nerr ++;
    }
    errno = errno_save;
}

void _log_stderr(const char *fmt, ...)
{
    int     len;
    char    buf[BZ_MAX_ERR_STR];
    va_list args;

    va_start(args, fmt);
    len = bz_vscnprintf(buf, BZ_MAX_ERR_STR, fmt, args);
    va_end(args);

    buf[len++] = '\n';
    write(STDERR_FILENO, buf, len);
}

#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
void bz_log_backtrace(bz_log_t *log, int level){
    void  *bt_buffer[32];
    size_t bt_size, i;
    char **bt_strings;

    bt_size    = backtrace(bt_buffer, sizeof(bt_buffer) / sizeof(bt_buffer[0]));
    bt_strings = backtrace_symbols(bt_buffer, bt_size);

    for (i = 0; i < bt_size; i++) {
        bz_log(log, level, "%s", bt_strings[i]);
    }

    free(bt_strings);
}
#endif

