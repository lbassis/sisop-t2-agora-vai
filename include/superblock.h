#ifndef __superblock__
#define __superblock__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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


typedef struct SUPERBLOCK {
  unsigned int diskSize;
  unsigned int numberOfSectors;
  unsigned int sectorsPerCluster;
  unsigned int fatSectorStart;
  unsigned int rootDirCluster;
  unsigned int dataSectorStart;
}SUPERBLOCK;

void diskId();
void readSuperBlock(SUPERBLOCK *super);
void printSuperBlock(SUPERBLOCK s);

#endif