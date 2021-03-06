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
 * Mark Ryan <mark.d.ryan@intel.com>
 *
 */

#include "async.h"
#include "error.h"
#include "log.h"

void msu_async_task_delete(msu_async_task_t *cb_data)
{
	switch (cb_data->task.type) {
	case MSU_TASK_GET_CHILDREN:
	case MSU_TASK_SEARCH:
		if (cb_data->ut.bas.vbs)
			g_ptr_array_unref(cb_data->ut.bas.vbs);
		break;
	case MSU_TASK_GET_ALL_PROPS:
	case MSU_TASK_GET_RESOURCE:
		if (cb_data->ut.get_all.vb)
			g_variant_builder_unref(cb_data->ut.get_all.vb);
		break;
	case MSU_TASK_UPLOAD_TO_ANY:
	case MSU_TASK_UPLOAD:
		g_free(cb_data->ut.upload.mime_type);
		break;
	case MSU_TASK_UPDATE_OBJECT:
		g_free(cb_data->ut.update.current_tag_value);
		g_free(cb_data->ut.update.new_tag_value);
		break;
	case MSU_TASK_CREATE_PLAYLIST:
	case MSU_TASK_CREATE_PLAYLIST_IN_ANY:
		g_free(cb_data->ut.playlist.didl);
		if (cb_data->ut.playlist.collection)
			g_object_unref(cb_data->ut.playlist.collection);
		break;
	default:
		break;
	}

	if (cb_data->cancellable)
		g_object_unref(cb_data->cancellable);
}

gboolean msu_async_task_complete(gpointer user_data)
{
	msu_async_task_t *cb_data = user_data;

	MSU_LOG_DEBUG("Enter. Error %p", (void *)cb_data->error);
	MSU_LOG_DEBUG_NL();

	if (cb_data->proxy != NULL)
		g_object_remove_weak_pointer((G_OBJECT(cb_data->proxy)),
					     (gpointer *)&cb_data->proxy);

	cb_data->cb(&cb_data->task, cb_data->error);

	return FALSE;
}

void msu_async_task_cancelled_cb(GCancellable *cancellable, gpointer user_data)
{
	msu_async_task_t *cb_data = user_data;

	if (cb_data->proxy != NULL)
		gupnp_service_proxy_cancel_action(cb_data->proxy,
						  cb_data->action);

	if (!cb_data->error)
		cb_data->error = g_error_new(MSU_ERROR, MSU_ERROR_CANCELLED,
					     "Operation cancelled.");
	(void) g_idle_add(msu_async_task_complete, cb_data);
}

void msu_async_task_cancel(msu_async_task_t *cb_data)
{
	if (cb_data->cancellable)
		g_cancellable_cancel(cb_data->cancellable);
}
