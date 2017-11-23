#ifndef __superblock__
#define __superblock__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <t2fs.h>


#define DISK_FILE "t2fs_disk.dat"

#define ID_SIZE 4

#define SUPER_BLOCK_SIZE_OFFSET 6
#define SUPER_BLOCK_SIZE 2

#define DISK_SIZE_OFFSET 8
#define DISK_SIZE 4

#define NUMBER_OF_SECTORS_OFFSET 12
#define NUMBER_OF_SECTORS_SIZE 4

#define SECTORS_PER_CLUSTER_OFFSET 16
#define SECTORS_PER_CLUSTER_SIZE 4

#define FAT_SECTOR_START_OFFSET 20
#define FAT_SECTOR_START_SIZE 4

#define ROOT_DIR_CLUSTER_OFFSET 24
#define ROOT_DIR_CLUSTER_SIZE 4

#define DATA_SECTOR_START_OFFSET 28
#define DATA_SECTOR_START_SIZE 4



void diskId();
void readSuperBlock(struct t2fs_superbloco *super);
void printSuperBlock(struct t2fs_superbloco s);

#endif
