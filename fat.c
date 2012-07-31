#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fat.h"

int get_fat_type(fat_device_t *device)
{
	int fat_type = 0;
	unsigned int cluster_count = device->cluster_count;
	
	if(cluster_count < FAT12_CLUSTER_COUNT) {
		fat_type = FAT12;
	} else if(cluster_count < FAT16_CLUSTER_COUNT) {
		fat_type = FAT16;
	} else {
		fat_type = FAT32;
	}
	
	device->fat_type = fat_type;
	
	return(0);
}

int get_root_dir_sectors(fat_device_t *device)
{
	fat_bs_t *fatbs = &device->boot_sect;
	unsigned int root_dir_sectors = 0;
	
	root_dir_sectors = ((fatbs->root_entry_count * 32) +
		(fatbs->bytes_per_sector - 1)) / fatbs->bytes_per_sector;
	
	device->root_dir_sectors = root_dir_sectors;
	
	return(0);
}

int get_fat_size(fat_device_t *device)
{
	fat_bs_t *fatbs = &device->boot_sect;
	fat_extbs_32_t *fatext32 = NULL;
	unsigned int fat_size = 0;
	
	if(fatbs->table_size_16 != 0) {
		fat_size = fatbs->table_size_16;
	} else {
		fatext32 = (fat_extbs_32_t *) fatbs->extended_section;
		fat_size = fatext32->table_size_32;
	}
	
	device->fat_size = fat_size;
	
	return(0);
}

int get_total_sectors(fat_device_t *device)
{
	fat_bs_t *fatbs = &device->boot_sect;
	unsigned int total_sec = 0;
	
	if(fatbs->total_sectors_16 != 0) {
		total_sec = fatbs->total_sectors_16;
	} else {
		total_sec = fatbs->total_sectors_32;
	}
	
	device->total_sec = total_sec;
	
	return(0);
}

int get_first_data_sector(fat_device_t *device)
{
	fat_bs_t *fatbs = &device->boot_sect;
	unsigned int first_data_sector = 0;
	
	first_data_sector = fatbs->reserved_sector_count +
		device->root_dir_sectors + fatbs->table_count * device->fat_size;
	
	device->first_data_sector = first_data_sector;
	
	return(0);
}

unsigned int get_first_sector_of_cluster(fat_device_t *device, unsigned int cluster)
{
	unsigned int first_sector_cluster = 0;
	
	first_sector_cluster = ((cluster - 2) * device->boot_sect.sectors_per_cluster)
		+ device->first_data_sector;
	
	return(first_sector_cluster);
}

int get_cluster_count(fat_device_t *device)
{
	unsigned int dataregion_sec_count = 0, cluster_count = 0;
	
	dataregion_sec_count = device->total_sec - (device->boot_sect.reserved_sector_count + 
	(device->boot_sect.table_count * device->fat_size) + device->root_dir_sectors);
	
	cluster_count = dataregion_sec_count / device->boot_sect.sectors_per_cluster;
	
	device->cluster_count = cluster_count;
	
	return(0);
}

/* TODO: Need to investigate about ThisFATSecNum, ThisFATEntOffset.
 * TODO: Fix or Remove the above TODO.
 */
unsigned int get_fat_offset(fat_device_t *device, unsigned int cluster)
{
	unsigned int fat_offset = 0;
	
	if(device->fat_type == FAT16) {
		fat_offset = cluster * 2;
	} else if(device->fat_type == FAT32) {
		fat_offset = cluster * 4;
	} else if(device->fat_type == FAT12) {
		fat_offset = cluster + cluster / 2;
	}
	
	return(fat_offset);
}


fat_device_t* fat_device_init(fat_bs_t* boot_sector)
{
	fat_device_t *device = NULL;
	
	device = calloc(1, sizeof(fat_device_t));
	
	if(device != NULL) {
		memcpy(&device->boot_sect, boot_sector, sizeof(fat_bs_t));
		
		get_root_dir_sectors(device);
		get_fat_size(device);
		get_total_sectors(device);
		get_cluster_count(device);
		get_fat_type(device);
		get_first_data_sector(device);
		
		return(device);
	}
	
	return(NULL);
}


void print_fat_info(fat_device_t* fatdevice)
{
	printf("FAT Type is FAT%d\n", fatdevice->fat_type);
}
