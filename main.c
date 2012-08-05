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

    fd = fopen("/home/prasanna/Projects/OpenSource/Fat_FileSystem/fatfs_minix/fat32.img", "r+b");
    // fd = fopen("/home/prasanna/Projects/OpenSource/Fat_FileSystem/fatfs_minix/fat16.img", "r+b");
	if(fd == NULL) {
		return (-1);
	}

    read_size = bdev_read(0, 0, buf, 512, 0);
    // read_size = fread(buf, 1, 512, fd);
	
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
	
    printf("OEM Name is %s\n", boot_sector.oemname);
    printf("Bytes per sector = %d\n", boot_sector.bytes_per_sector);
    printf("Sectors per cluster = %d\n", boot_sector.sectors_per_cluster);
    printf("Sectors per track = %d\n", boot_sector.sectors_per_track);

    print_fat_info(dev);

    if(dev->fat_type == FAT32) {
        fat_extbs_32_t fat32 = {0};
        unsigned int fsinfo_sector = 1;
        fat32_fsinfo_t fsinfo = {0};

        memcpy(&fat32, dev->boot_sect.extended_section, sizeof(fat_extbs_32_t));
        fsinfo_sector = fat32.fat_info;

        bdev_read(0, fsinfo_sector * 512, (char *)&fsinfo, 512, 0);
        get_fat32_fsinfo(&fsinfo, dev);

        printf("FAT32 Free cluster count = %d\n", dev->free_cluster_count);
        printf("FAT32 Next free cluster = %d\n", dev->next_free_cluster);
    }

    fclose(fd);
	
	return(0);
}

