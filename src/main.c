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

#include <getopt.h>
#include <sys/utsname.h>

#include "bitzer.h"

#define BZ_LOG_DEFAULT   BZ_LOG_INFO

#ifndef BZ_LOG_PATH
# define BZ_LOG_PATH     "logs/bitzer.log"
#endif

#ifndef BZ_PID_PATH
# define BZ_PID_PATH     "logs/bitzer.pid"
#endif

#ifndef BZ_CONF_PATH
# define BZ_CONF_PATH    "conf/bitzer.conf"
#endif

static int show_help;
static int show_version;
static int test_conf;
static int daemonize;

static struct option long_options[] = {
    { "help",           no_argument,        NULL,   'h' },
    { "version",        no_argument,        NULL,   'v' },
    { "test-conf",      no_argument,        NULL,   't' },
    { "not-daemonize",  no_argument,        NULL,   'D' },
    { "prefix",         required_argument,  NULL,   'p' },
    { "verbose",        required_argument,  NULL,   'l' },
    { "conf-file",      required_argument,  NULL,   'c' },
    { "log-file",       required_argument,  NULL,   'L' },
    { "pid-file",       required_argument,  NULL,   'P' },
    { NULL,             0,                  NULL,    0  }
};

static char short_options[] = "hvtDp:l:c:L:P:";

static int bz_get_options(int argc, char *argv[], struct bitzer_s *bz)
{
    int c, value;

    opterr = 0;

    // default it is a daemon
    daemonize = 1;

    for (;;) {
        c = getopt_long(argc, argv, short_options, long_options, NULL);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
            show_version = 1;
            show_help    = 1;
            break;

        case 'v':
            show_version = 1;
            break;

        case 't':
            test_conf = 1;
            break;

        case 'D':
            daemonize = 0;
            break;

        case 'p':
            bz->prefix = optarg;
            if (access(bz->prefix, X_OK) < 0) {
                bz_log_stderr("access prefix directory (%s) failed: %s",
                              bz->prefix, strerror(errno));
                return BZ_ERROR;
            }
            break;

        case 'l':
            value = atoi(optarg);
            if (value < 0) {
                bz_log_stderr(BITZER_NAME ": option -l requires a number");
                return BZ_ERROR;
            }
            bz->log_level = value;
            break;

        case 'c':
            bz->conf_file = optarg;
            break;

        case 'L':
            bz->log_file  = optarg;
            break;

        case 'P':
            bz->pid_file  = optarg;
            break;

        case '?':
            switch (optopt) {
            case 'p':
                bz_log_stderr(BITZER_NAME ": option -%c requires a "
                              "directory name", optopt);
                break;

            case 'c':
            case 'L':
            case 'P':
                bz_log_stderr(BITZER_NAME ": option -%c requires a file name",
                              optopt);
                break;

            case 'l':
                bz_log_stderr(BITZER_NAME ": option -%c requires a number",
                              optopt);
                break;

            default:
                bz_log_stderr(BITZER_NAME ": invalid option '-%c'", optopt);
                break;
            }
            return BZ_ERROR;

        default:
            bz_log_stderr(BITZER_NAME ": invalid option '-%c'", optopt);
            return BZ_ERROR;
        }
    }

    return BZ_OK;
}

static void bz_show_usage()
{
    bz_log_stderr(
        "Usage: " BITZER_NAME " [-hvtD] [-p prefix] [-l log_level] "
        "[-L log_file] [-P pid_file]" BZ_LINEFEED
    );
    bz_log_stderr(
        "Options:" BZ_LINEFEED
        "  -h, --help              : this help" BZ_LINEFEED
        "  -v, --version           : show version and exit" BZ_LINEFEED
        "  -t, --test-conf         : test configuration and exit" BZ_LINEFEED
        "  -D, --not-daemonize     : do not daemonize" BZ_LINEFEED
        "  -p, --prefix            : set prefix path (default: "
                                     BZ_INSTALL_PREFIX ")" BZ_LINEFEED
        "  -l, --verbose           : set log level (default: %d, min: %d "
                                     "(less verbose), max: %d (more verbose))"
                                     BZ_LINEFEED
        "  -c, --conf-file         : set configuration file (default: "
                                     BZ_CONF_PATH ")" BZ_LINEFEED
        "  -L, --log-file          : set log file (default: "
                                     BZ_LOG_PATH ")" BZ_LINEFEED
        "  -P, --pid-file          : set pid file (default: "
                                     BZ_PID_PATH ")"
        , BZ_LOG_DEFAULT, BZ_LOG_EMERG, BZ_LOG_VERB
    );
}

static int bz_daemonize(bz_log_t *log)
{
    pid_t pid;

    pid = fork();
    switch(pid) {
    case  0:
        break;
    case -1:
        bz_log_error(log, "fork() failed: %s", strerror(errno));
        return BZ_ERROR;
    default:
        // in parent
        _exit(0);
    }

    if (setsid() < 0) {
        bz_log_error(log, "setsid() failed: %s", strerror(errno));
        return BZ_ERROR;
    }

    // fork again
    pid = fork();
    switch(pid) {
    case  0:
        break;
    case -1:
        bz_log_error(log, "fork() again failed: %s", strerror(errno));
        return BZ_ERROR;
    default:
        _exit(0);
    }

    return BZ_OK;
}

static int bz_redirect_io(struct bitzer_s *bz)
{
    int   fd;
    char *ptr;

    umask(0);

    fd = open(bz->log_file, O_RDWR | O_APPEND);
    if (fd < 0) {
        bz_log_error(bz->log, "open(\"%s\") failed: %s",
                     bz->log_file, strerror(errno));
        return BZ_ERROR;
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

    if (close(fd) == -1) {
        bz_log_error(bz->log, "close(\"%s\") failed: %s",
                     bz->log_file, strerror(errno));
        return BZ_ERROR;
    }
    return BZ_OK;

DUP2FAILED:
    bz_log_error(bz->log, "dup2(%d, \"%s\") failed: %s", fd, ptr, strerror(errno));
    close(fd);
    return BZ_ERROR;
}

static int bz_write_pidfile(struct bitzer_s *bz)
{
    int fd, len;
    char pid[BZ_INT64_LEN + 1];

    fd = open(bz->pid_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        bz_log_error(bz->log, "open pid file '%s' failed: %s",
                     bz->pid_file, strerror(errno));
        return BZ_ERROR;
    }

    len = snprintf(pid, BZ_INT64_LEN + 1, "%d", bz->pid);
    if (len < 0) {
        close(fd);
        return BZ_ERROR;
    }

    pid[len++] = '\n';
    if (write(fd, pid, len) < 0) {
        bz_log_error(bz->log, "write pid file '%s' failed: %s",
                     bz->pid_file, strerror(errno));
        close(fd);
        return BZ_ERROR;
    }

    close(fd);
    return BZ_OK;
}

static int bz_remove_pidfile(struct bitzer_s *bz)
{
    return (unlink(bz->pid_file) < 0) ? BZ_ERROR : BZ_OK;
}

static void bz_set_default_instance(struct bitzer_s *bz)
{
    bz->prefix    = BZ_INSTALL_PREFIX;
    bz->log_level = BZ_LOG_DEFAULT;
    bz->log_file  = BZ_LOG_PATH;

    bz->conf_file = BZ_CONF_PATH;

    if (gethostname(bz->hostname, BZ_MAXHOSTNAMELEN)) {
        bz_log_warn(bz->log, "gethostname() failed: %s", strerror(errno));
        strncpy(bz->hostname, "unknown", sizeof("unknown")); // no -1 here
    }
    bz->hostname[BZ_MAXHOSTNAMELEN - 1] = '\0';

    bz->pid      = (pid_t)-1;
    bz->pid_file = BZ_PID_PATH;

    bz->log_file_alloc  = 0;
    bz->conf_file_alloc = 0;
    bz->pid_file_alloc  = 0;
}

static void bz_print_sysinfo(struct bitzer_s *bz)
{
    struct utsname uts;

    bz_log(bz->log, BZ_LOG_INFO, "bitzer version: " BITZER_VERSION);

    if (uname(&uts) < 0) {
        return;
    }
    bz_log(bz->log, BZ_LOG_INFO, "OS: %s %s %s", uts.sysname,
           uts.release, uts.machine);
}

#define BZ_MEMBER_ALLOC(_member) _member ## _alloc

#define BZ_STRCAT_IF_RELATIVE(_instance, _member, _path)   \
do {                                                       \
    if (_instance->_member[0] != BZ_PATHSEP) {             \
        plen = strlen(bz->prefix);                         \
        name = malloc(plen + strlen(_path) + 2);           \
        if (name == NULL) {                                \
            return BZ_ERROR;                               \
        }                                                  \
        p = memcpy(name, bz->prefix, plen);                \
        p += plen;                                         \
        if (*(p - 1) != BZ_PATHSEP) {                      \
            *p++ = BZ_PATHSEP;                             \
        }                                                  \
        memcpy(p, _path, strlen(_path) + 1);               \
        _instance->_member = name;                         \
        _instance->BZ_MEMBER_ALLOC(_member) = 1;           \
    }                                                      \
} while(0)

static int bz_init_instance(struct bitzer_s *bz)
{
    size_t plen;
    char *p, *name;

    BZ_STRCAT_IF_RELATIVE(bz, log_file,  bz->log_file);
    BZ_STRCAT_IF_RELATIVE(bz, conf_file, bz->conf_file);
    BZ_STRCAT_IF_RELATIVE(bz, pid_file,  bz->pid_file);

    bz->log = bz_log_create(bz->log_level, bz->log_file);
    if (!bz->log) {
        return BZ_ERROR;
    }

    bz_print_sysinfo(bz);

    if (daemonize && bz_daemonize(bz->log) != BZ_OK) {
        return BZ_ERROR;
    }

    if (daemonize && bz_redirect_io(bz) != BZ_OK) {
        return BZ_ERROR;
    }

    if (signal_init(bz->log) != BZ_OK) {
        return BZ_ERROR;
    }

    bz->pid = getpid();
    if (bz_write_pidfile(bz) != BZ_OK) {
        return BZ_ERROR;
    }

    return BZ_OK;
}

#define BZ_FREE_IF_ALLOC(_instance, _member)    \
do {                                            \
    if (_instance->BZ_MEMBER_ALLOC(_member)) {  \
        free(_instance->_member);               \
        _instance->_member = NULL;              \
    }                                           \
} while(0)

static void bz_post_run(struct bitzer_s *bz)
{
    signal_close(bz->log);

    if (bz_remove_pidfile(bz) != BZ_OK && bz->log) {
        bz_log_error(bz->log, "remove pid file '%s' failed: %s",
                     bz->pid_file, strerror(errno));
    }

    if (bz->log) {
        bz_log(bz->log, BZ_LOG_INFO, "exit");
        bz_log_close(bz->log);
    }

    BZ_FREE_IF_ALLOC(bz, log_file);
    BZ_FREE_IF_ALLOC(bz, conf_file);
    BZ_FREE_IF_ALLOC(bz, pid_file);
}

static void bz_signal_callback(void *arg)
{
    // not graceful termination
    if (bz_quit) {
        exit(2);
    }

    // graceful termination
    if (bz_terminate) {
        // here `context` is global variable
        context_close(context);
        bz_post_run((struct bitzer_s *)arg);
        exit(1);
    }
}

static void bz_run(struct bitzer_s *bz)
{
    context_t *ctx;

    ctx = context_create(bz);
    if (ctx == NULL) {
        return;
    }

    if (context_init(ctx) != BZ_OK) {
        return;
    }

    context = ctx;
    context_set_signal_callback(ctx, bz_signal_callback, (void *)bz);
    context_run(ctx);

    context_close(ctx);
}

int main(int argc, char *argv[])
{
    int status;
    struct bitzer_s bz;

    bz_set_default_instance(&bz);

    if (bz_get_options(argc, argv, &bz) != BZ_OK) {
        bz_show_usage();
        exit(1);
    }

    if (show_version) {
        bz_log_stderr(BITZER_NAME ": " BITZER_VERSION);
        if (show_help) {
            bz_show_usage();
        }
        exit(0);
    }

    status = bz_init_instance(&bz);
    if (status != BZ_OK) {
        bz_post_run(&bz);
        exit(1);
    }

    bz_run(&bz);

    bz_post_run(&bz);
    exit(1);
}
