/*
 * media-service-upnp
 *
 * Copyright (C) 2012 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Regis Merlino <regis.merlino@intel.com>
 *
 */

#ifndef MSU_TASK_ATOM_H__
#define MSU_TASK_ATOM_H__

typedef struct msu_task_queue_key_t_ msu_task_queue_key_t;

typedef struct msu_task_atom_t_ msu_task_atom_t;
struct msu_task_atom_t_ {
	const msu_task_queue_key_t *queue_id;
};

#endif /* MSU_TASK_ATOM_H__ */
