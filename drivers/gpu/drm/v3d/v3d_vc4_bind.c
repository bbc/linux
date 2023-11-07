#define DEBUG
#include <linux/fdtable.h>
#include <drm/drm_drv.h>
#include <drm/drm_prime.h>
#include "v3d_vc4_bind.h"
#include "../vc4/vc4_drv.h"

#include "uapi/drm/vc4_drm.h"

int import_bo_from_vc4(struct drm_device *dev, struct drm_file *file_priv,
		       size_t unaligned_size, u32 *handle)
{
	struct drm_vc4_create_bo args = { 0 };
	int ret, prime_fd = 0;
	u32 vc4_handle = 0;

	if (!is_vc4_enable())
		return -ENODEV;

	args.size = unaligned_size;
	args.flags = ARC_CALLOC;
	ret = vc4_create_bo_ioctl(vc4_drm, &args, vc4_drm_file);
	if (ret)
		goto out;
	vc4_handle = args.handle;
	ret = drm_gem_prime_handle_to_fd(vc4_drm, vc4_drm_file, args.handle,
					 DRM_CLOEXEC | DRM_RDWR, &prime_fd);
	if (ret)
		ret = drm_gem_prime_handle_to_fd(vc4_drm, vc4_drm_file,
						 args.handle, DRM_CLOEXEC,
						 &prime_fd);
	if (ret)
		goto out;

	ret = drm_gem_prime_fd_to_handle(dev, file_priv, prime_fd, handle);

out:
	if (prime_fd) {
		close_fd(prime_fd);
	}
	if (vc4_handle)
		drm_gem_handle_delete(vc4_drm_file, vc4_handle);
	if (ret)
		DRM_ERROR(
			"Borrow bo from vc4 size:%uk, vc4 handle:%u, fd:%d, v3d handle:%u, ret:%d\n",
			(u32)(unaligned_size / 1024), vc4_handle, prime_fd,
			*handle, ret);
	return ret;
}
