/* This code is licensed under LGPL v2 or later and BSD license.
 * Refer to LICENSE file for terms and conditions.
 *
 * Copyright (C) 2012 Prasanna Kumar T S M <prasannatsmkumar@gmail.com>
 */

#include <stdio.h>
#include <string.h>

#include "fat.h"
#include "minix_compat.h"
#include "dev.h"

FILE *fd = NULL;

int main(void)
{
	fat_bs_t boot_sector;
	char buf[512];
	int read_size = 0;
	
	fat_device_t *dev = NULL;
	
	memset(&boot_sector, 0, sizeof(boot_sector));
	memset(buf, 0, sizeof(buf));

	fd = fopen("/home/prasanna/Projects/OpenSource/Fat_FileSystem/fatfs_minix/fat16.img", "r+b");
	if(fd == NULL) {
		return (-1);
	}

    read_size = bdev_read(0, 0, buf, 512, 0);
    // read_size = fread(buf, 1, 512, fd);
	
	fclose(fd);
	
	if(read_size < sizeof(fat_bs_t)) {
		printf("\n");
	}
	
	if(buf[510] != 0x55 || buf[511] != 0xAA) {
		/* Atari formatted disc may not obey this - http://en.wikipedia.org/wiki/File_Allocation_Table */
		printf("***** FAT Boot Parameter Block Signature missing *****\n");
	}

	memcpy(&boot_sector, buf, sizeof(fat_bs_t));
	
	dev = fat_device_init(&boot_sector);
	if(dev == NULL) {
		printf("Insufficient Memory\n");
		return(-2);
	}
	
	printf("%s\n", boot_sector.oemname);
	printf("%d\n", boot_sector.bytes_per_sector);
	printf("%d\n", boot_sector.sectors_per_cluster);
	printf("%d\n", boot_sector.sectors_per_track);
	
	print_fat_info(dev);
	
	return(0);
}

