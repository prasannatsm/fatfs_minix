#ifndef MINIX_COMPAT_H
#define MINIX_COMPAT_H

#include <stdlib.h>

typedef unsigned int uint32_t;
// typedef uint32_t dev_t;
// typedef size_t ssize_t;

/* ssize_t bdev_read(dev_t dev, u64_t pos, char *buf, size_t count, int flags) */
ssize_t bdev_read(dev_t dev, unsigned long position, char *buf, size_t count, int flags);

/* ssize_t bdev_write(dev_t dev, u64_t pos, char *buf, size_t count, int flags) */
ssize_t bdev_write(dev_t dev, unsigned long position, char *buf, size_t count, int flags);

#endif /* MINIX_COMPAT_H */
