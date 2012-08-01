/* This code is licensed under LGPL v2 or later and BSD license.
 * Refer to LICENSE file for terms and conditions.
 *
 * Copyright (C) 2012 Prasanna Kumar T S M <prasannatsmkumar@gmail.com>
 */

#include <stdio.h>

#include "minix_compat.h"

extern FILE *fd;

ssize_t bdev_read(dev_t dev, unsigned long position, char *buf, size_t count, int flags)
{
	unsigned long size = 0;
	
	(dev);
	(flags);
	
	fseek(fd, position, SEEK_SET);
	size = fread(buf, 1, count, fd);
	
	return(size);
}

/* TODO: Check if bdev_write works correctly. */
ssize_t bdev_write(dev_t dev, unsigned long position, char *buf, size_t count, int flags)
{
	unsigned long size = 0;
	
	(dev);
	(flags);
	
	fseek(fd, position, SEEK_SET);
	size = fwrite(buf, 1, count, fd);
	
	return(size);
}
