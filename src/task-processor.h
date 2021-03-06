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

#ifndef MSU_TASK_PROCESSOR_H__
#define MSU_TASK_PROCESSOR_H__

#include <gio/gio.h>
#include <glib.h>

#include "task-atom.h"

enum msu_task_queue_flag_mask_ {
	MSU_TASK_QUEUE_FLAG_NONE = 0,
	MSU_TASK_QUEUE_FLAG_AUTO_START = 1,
	MSU_TASK_QUEUE_FLAG_AUTO_REMOVE = 1 << 1,
};
typedef enum msu_task_queue_flag_mask_ msu_task_queue_flag_mask;

typedef struct msu_task_processor_t_ msu_task_processor_t;

typedef void (*msu_task_process_cb_t)(msu_task_atom_t *task,
				      gpointer user_data);
typedef void (*msu_task_cancel_cb_t)(msu_task_atom_t *task,
				     gpointer user_data);
typedef void (*msu_task_delete_cb_t)(msu_task_atom_t *task,
				     gpointer user_data);
typedef void (*msu_task_finally_cb_t)(gboolean cancelled, gpointer user_data);

msu_task_processor_t *msu_task_processor_new(GSourceFunc on_quit_cb);
void msu_task_processor_free(msu_task_processor_t *processor);
void msu_task_processor_set_quitting(msu_task_processor_t *processor);
const msu_task_queue_key_t *msu_task_processor_add_queue(
					msu_task_processor_t *processor,
					const gchar *source,
					const gchar *sink,
					guint32 flags,
					msu_task_process_cb_t task_process_cb,
					msu_task_cancel_cb_t task_cancel_cb,
					msu_task_delete_cb_t task_delete_cb);
const msu_task_queue_key_t *msu_task_processor_lookup_queue(
					const msu_task_processor_t *processor,
					const gchar *source,
					const gchar *sink);
void msu_task_processor_cancel_queue(const msu_task_queue_key_t *queue_id);
void msu_task_processor_remove_queues_for_source(
						msu_task_processor_t *processor,
						const gchar *source);
void msu_task_processor_remove_queues_for_sink(msu_task_processor_t *processor,
					       const gchar *sink);

void msu_task_queue_start(const msu_task_queue_key_t *queue_id);
void msu_task_queue_add_task(const msu_task_queue_key_t *queue_id,
			     msu_task_atom_t *task);
void msu_task_queue_task_completed(const msu_task_queue_key_t *queue_id);
void msu_task_queue_set_finally(const msu_task_queue_key_t *queue_id,
				msu_task_finally_cb_t finally_cb);
void msu_task_queue_set_user_data(const msu_task_queue_key_t *queue_id,
				  gpointer user_data);
gpointer msu_task_queue_get_user_data(const msu_task_queue_key_t *queue_id);

#endif /* MSU_TASK_PROCESSOR_H__ */
