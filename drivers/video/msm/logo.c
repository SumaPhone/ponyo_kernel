/* drivers/video/msm/logo.c
 *
 * Show Logo in RLE 565 format
 *
 * Copyright (C) 2008 Google Incorporated
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fb.h>
#include <linux/vt_kern.h>
#include <linux/unistd.h>
#include <linux/syscalls.h>

#include <linux/irq.h>
#include <asm/system.h>

#define fb_width(fb)	((fb)->var.xres)
#define fb_height(fb)	((fb)->var.yres)
#define fb_size(fb)	((fb)->var.xres * (fb)->var.yres * 2)

#define RGB565_R(v) ((((u32)v)&(0x1F<<11))>>11)
#define RGB565_G(v) ((((u32)v)&(0x3F<<5))>>5)
#define RGB565_B(v) ((((u32)v)&(0x1F<<0))>>0)

#define RGB565_TO_A888(v) ((RGB565_R(v)<<3)|(RGB565_G(v)<<(8+2))|(RGB565_B(v)<<(16+3)))

typedef int (*pixel_set_t)(void *,unsigned short, unsigned);

static int pixel_set_rgba(void *_ptr, unsigned short val, unsigned count)
{
	u32 *ptr = _ptr;
    int size = count << 2;
    
	while (count--)
		*ptr++ = RGB565_TO_A888(val); // why the format is ABGR?

    return size;
}

static int memset16(void *_ptr, unsigned short val, unsigned count)
{
	unsigned short *ptr = _ptr;
    int size = count << 1;
    
	while (count--)
		*ptr++ = val;

    return size;
}

/* 565RLE image format: [count(2 bytes), rle(2 bytes)] */
int load_565rle_image(char *filename)
{
	struct fb_info *info;
	int fd, count, err = 0;
	unsigned max;
	unsigned short *data, *ptr;
    char *bits;
    pixel_set_t pixel_set;

	info = registered_fb[0];
	if (!info) {
		printk(KERN_WARNING "%s: Can not access framebuffer\n",
			__func__);
		return -ENODEV;
	}
    
    switch (info->var.bits_per_pixel) {
    case 16:
        pixel_set = memset16;
        break;
    case 32:
        pixel_set = pixel_set_rgba;
        break;
    default:
		printk(KERN_WARNING "%s: Can not find pixel_set operation\n",
			__func__);    
		return -EDOM;
    }

	fd = sys_open(filename, O_RDONLY, 0);
	if (fd < 0) {
		printk(KERN_WARNING "%s: Can not open %s\n",
			__func__, filename);
		return -ENOENT;
	}
	count = sys_lseek(fd, (off_t)0, 2);
	if (count <= 0) {
		err = -EIO;
		goto err_logo_close_file;
	}
	sys_lseek(fd, (off_t)0, 0);
	data = kmalloc(count, GFP_KERNEL);
	if (!data) {
		printk(KERN_WARNING "%s: Can not alloc data\n", __func__);
		err = -ENOMEM;
		goto err_logo_close_file;
	}
	if (sys_read(fd, (char *)data, count) != count) {
		err = -EIO;
		goto err_logo_free_data;
	}

	max = fb_width(info) * fb_height(info);
	ptr = data;
	bits = info->screen_base;
	while (count > 3) {
		unsigned n = ptr[0];
		if (n > max)
			break;
		bits += pixel_set(bits, ptr[1], n);
		max -= n;
		ptr += 2;
		count -= 4;
	}

err_logo_free_data:
	kfree(data);
err_logo_close_file:
	sys_close(fd);
	return err;
}
EXPORT_SYMBOL(load_565rle_image);
