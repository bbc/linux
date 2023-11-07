// SPDX-License-Identifier: GPL-2.0+
/* Copyright (C) 2020 FydeOS team */
/* Author: Yang Tsao (yang@fydeos.io)*/

#ifndef V3D_VC4_BIND_H
#define V3D_VC4_BIND_H
#include <drm/drm_device.h>
#include <drm/drm_file.h>

#define ARC_CALLOC (1 << 7)
extern struct drm_device *vc4_drm;
extern struct drm_file *vc4_drm_file;

static inline bool is_vc4_enable(void)
{
	return vc4_drm != NULL && vc4_drm_file != NULL;
}

int import_bo_from_vc4(struct drm_device *dev, struct drm_file *file_priv,
		       size_t unaligned_size, u32 *handle);
#endif /* V3D_VC4_BIND_H */
