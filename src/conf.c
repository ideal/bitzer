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

conf_t *conf_create()
{
    conf_t *cnf;

    cnf = (conf_t *)malloc(sizeof(conf_t));
    if (cnf) {
        // an empty tasks list
        INIT_LIST_HEAD(&cnf->tasks_list);
    }
    
    return cnf;
}

int conf_load(const char *path)
{
    return OK;
}

int conf_close(conf_t *cnf)
{
    return OK;
}
