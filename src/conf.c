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
#include "conf_gram.h"
#include "conf_scan.h"

int yyparse(conf_t *cnf, yyscan_t scanner);
static char *conf_mmap(const char *path, size_t *len);

conf_t *conf_create(context_t *ctx)
{
    conf_t *cnf;

    cnf = (conf_t *)bz_alloc(sizeof(conf_t), ctx->log);
    if (cnf) {
        // an empty tasks list
        INIT_LIST_HEAD(&cnf->tasks_list);
    }

    return cnf;
}

int conf_load(conf_t *cnf, const char *path)
{
    size_t len;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
    const char *confstr;

    if (yylex_init(&scanner) != 0) {
        return ERROR;
    }

    if (!(confstr = conf_mmap(path, &len))) {
        return ERROR;
    }

    state = yy_scan_bytes(confstr, len, scanner);
    if (yyparse(cnf, scanner) != 0) {
        munmap((void *)confstr, len);
        return ERROR;
    }

    munmap((void *)confstr, len);
    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);

    return OK;
}

int conf_close(conf_t *cnf)
{
    // TODO: free tasks

    if (cnf) {
        free(cnf);
    }
    return OK;
}

static char *conf_mmap(const char *path, size_t *len)
{
    int fd;
    char *ptr;
    struct stat st;

    ptr = NULL;
    fd  = open(path, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    if (fstat(fd, &st) < 0) {
        goto EXIT;
    }

    *len = st.st_size;
    ptr  = (char *)mmap(NULL, *len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        ptr = NULL;
        goto EXIT;
    }

EXIT:
    close(fd);
    return ptr;
}
