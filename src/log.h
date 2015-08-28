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

#ifndef BZ_LOG_H
#define BZ_LOG_H

#include "config.h"
#include "util.h"

#define BZ_LOG_EMERG     0
#define BZ_LOG_ALERT     1
#define BZ_LOG_CRIT      2
#define BZ_LOG_ERR       3
#define BZ_LOG_ERROR     BZ_LOG_ERR
#define BZ_LOG_WARN      4
#define BZ_LOG_WARNING   BZ_LOG_WARN
#define BZ_LOG_NOTICE    5
#define BZ_LOG_INFO      6
#define BZ_LOG_DEBUG     7
#define BZ_LOG_VERB      8

#define BZ_MAX_ERR_STR   1024

struct bz_log_s {
    int    log_level;
    char  *file;
    int    fd;
    size_t nerr;
};

typedef struct bz_log_s bz_log_t;

#ifdef BZ_DEBUG_LOG

#define bz_log_debug(log, level, ...) do {          \
    if ((log)->log_level >= level)                  \
        _log(log, __FILE__, __LINE__, level,        \
             __VA_ARGS__);                          \
} while(0)

#else

#define bz_log_debug(log, level, ...)

#endif

#define bz_log_stderr(...)                          \
    _log_stderr(__VA_ARGS__)

#define bz_log(log, level, ...) do {                \
    if ((log)->log_level >= level)                  \
        _log(log, __FILE__, __LINE__, level,        \
             __VA_ARGS__);                          \
} while(0)

#define bz_log_warn(log, ...) do {                  \
    if ((log)->log_level >= BZ_LOG_WARN)            \
        _log(log, __FILE__, __LINE__, BZ_LOG_WARN,  \
             __VA_ARGS__);                          \
} while(0)
 
#define bz_log_error(log, ...) do {                 \
    if ((log)->log_level >= BZ_LOG_ERROR)           \
        _log(log, __FILE__, __LINE__,BZ_LOG_ERR,    \
             __VA_ARGS__);                          \
} while(0)

#define bz_abort(log, ...) do {                     \
    if ((log)->log_level >= BZ_LOG_EMERG)           \
        _log(log, __FILE__, __LINE__, BZ_LOG_EMERG, \
             __VA_ARGS__);                          \
    bz_log_backtrace(log, BZ_LOG_EMERG);            \
    abort();                                        \
} while(0)

#if defined(HAVE_BACKTRACE) && defined(HAVE_BACKTRACE_SYMBOLS)
  void bz_log_backtrace(bz_log_t *log, int level);
#else
# define bz_log_backtrace(log, level)
#endif

bz_log_t *bz_log_create(int level, const char *filename);
void  bz_log_close(bz_log_t  *log);
void  bz_log_reopen(bz_log_t *log);
void _log(bz_log_t *log, const char *file, int line, int level,
          const char *fmt, ...) __attribute__((format(printf, 5, 6)));
void _log_stderr(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif
