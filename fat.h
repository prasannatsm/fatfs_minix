#ifndef FAT_H
#define FAT_H

#include <sys/types.h>

#define FAT12 12
#define FAT16 16
#define FAT32 32

#define FAT12_CLUSTER_COUNT 4085
#define FAT16_CLUSTER_COUNT 65525
#define FAT32_CLUSTER_COUNT 4294967285

typedef struct fat_bs
{
    unsigned char bootjump[3]; // uint8_t
    unsigned char oemname[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sector_count;
    unsigned char table_count; // Number of FATs.
    unsigned short root_entry_count;
    unsigned short total_sectors_16;
    unsigned char media_type;
    unsigned short table_size_16;
    unsigned short sectors_per_track;
    unsigned short head_side_count;
    unsigned int hidden_sector_count;
    unsigned int total_sectors_32;

    /* The variable extended_section depends on the type of FAT.
     * 26 bytes for FAT12, FAT16 ,54 bytes for FAT32.
     *
     * This holds the Extended Boot Sector of FAT file system.
     */
    unsigned char extended_section[54];
} __attribute__((packed)) fat_bs_t;

typedef struct fat_extbs_16
{
    /* extended fat12 and fat16 stuff */
    unsigned char bios_drive_num;
    unsigned char reserved1;
    unsigned char boot_signature;
    unsigned int volume_id;
    unsigned char volume_label[11];
    unsigned char fat_type_label[8];
}__attribute__((packed)) fat_extbs_16_t;

typedef struct fat_extbs_32
{
    /* extended fat32 stuff */
    unsigned int table_size_32;
    unsigned short extended_flags;
    unsigned short fat_version;
    unsigned int root_cluster;
    unsigned short fat_info;
    unsigned short backup_BS_sector;
    unsigned char reserved_0[12];
    unsigned char drive_number;
    unsigned char reserved_1;
    unsigned char boot_signature;
    unsigned int volume_id;
    unsigned char volume_label[11];
    unsigned char fat_type_label[8];
}__attribute__((packed)) fat_extbs_32_t;

typedef struct fat_device
{
	fat_bs_t boot_sect;
	unsigned int fat_type;
	unsigned int root_dir_sectors;
	unsigned int fat_size;
	unsigned int first_data_sector;
	unsigned int first_sector_of_cluster;
    unsigned int total_sec;
    unsigned int cluster_count; // Num of data cluster starting at cluster 2.
} fat_device_t;

fat_device_t *fat_device_init(fat_bs_t *boot_sector);
void print_fat_info(fat_device_t *fatdevice);

int get_cluster_count(fat_device_t *device);
int get_first_data_sector(fat_device_t *device);
int get_total_sectors(fat_device_t *device);
int get_root_dir_sectors(fat_device_t* device);
int get_fat_size(fat_device_t* device);
int get_fat_type(fat_device_t* device);

unsigned int get_first_sector_of_cluster(fat_device_t *device, unsigned int cluster);
unsigned int get_fat_offset(fat_device_t *device, unsigned int cluster);

/* File attributes */
typedef struct fat_file_attr {
    unsigned int read_only:1;
    unsigned int hidden:1;
    unsigned int system:1;
    unsigned int vol:1;
    unsigned int dir:1;
    unsigned int archive:1;
    unsigned int device:1;
    unsigned int unused:1;
}__attribute__ ((packed)) fat_file_attr_t;

/* File date */
typedef struct fat_file_date {
    unsigned int day:5;
    unsigned int month:4;
    unsigned int year:7;
}__attribute__ ((packed)) fat_file_date_t;

/* File time */
typedef struct fat_file_time {
    unsigned int sec:5;
    unsigned int min:6;
    unsigned int hour:5;
}__attribute__ ((packed)) fat_file_time_t;

/* File info */
typedef struct fat_file {
    unsigned char name[8];              // 00 08
    unsigned char ext[3];               // 08 03
    unsigned char attr;                 // 11 01
    unsigned char winnt_flags;          // 12 01
    unsigned char creattimesecs;        // 13 01
    unsigned short creattime;           // 14 02
    unsigned short creatdate;           // 16 02
    unsigned short lastacces;           // 18 02
    unsigned short h_firstclus;         // 20 02
    fat_file_time_t lm_time;            // 22 02
    fat_file_date_t lm_date;            // 24 02
    unsigned short l_firstclus;         // 26 02
    unsigned int size;                  // 28 04
}__attribute__((packed)) fat_file_t;

/* Long file info */
typedef struct fat_file_long {
    unsigned char order;                // 00 01
    unsigned short first_5[5];          // 01 10
    unsigned char attr;                 // 11 01
    unsigned char long_entry_type;      // 12 01
    unsigned char checksum;             // 13 01
    unsigned char next_6[12];           // 14 12
    unsigned short zeros;               // 26 02
    unsigned char final_2[4];           // 28 04
}__attribute__((packed)) fat_file_long_t;

#endif /* FAT_H */
