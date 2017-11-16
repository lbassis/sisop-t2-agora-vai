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
#define NUMBER_OF_SECTORS_SIZE 4 // na especificaçao diz 4 mas deve ta errado!!

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

unsigned int hexToInt(unsigned char *buffer, int size) { // da pra generalizar isso pra deixar mais bonito

  unsigned int value;

  if (size == 4) {
    value = (buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);
  }

  else if (size == 2) {
    value = (buffer[1] << 8 | buffer[0]);
  }
  
  return value;
}


void diskId() { // só printa mesmo

  char id[ID_SIZE+1];
  FILE *file = fopen(DISK_FILE, "r");

  fseek(file, 0, SEEK_SET);

  fread(id, 1, ID_SIZE, file);
  id[ID_SIZE] = (char) 0; // marca de final da string
  printf("ID: %s\n", id);

  fclose(file);
}

unsigned int superBlockSize(FILE *file) {

  unsigned char buffer[SUPER_BLOCK_SIZE];

  fseek(file, SUPER_BLOCK_SIZE_OFFSET, SEEK_SET);
  fread(buffer, 1, SUPER_BLOCK_SIZE, file);

  return hexToInt(buffer, SUPER_BLOCK_SIZE);
}

unsigned int readDiskSize(FILE *file) {

  unsigned char buffer[DISK_SIZE];

  fseek(file, DISK_SIZE_OFFSET, SEEK_SET);
  fread(buffer, 1, DISK_SIZE, file);

  return hexToInt(buffer, DISK_SIZE);
}

unsigned int readNumberOfSectors(FILE *file) {

  unsigned char buffer[NUMBER_OF_SECTORS_SIZE];

  fseek(file, NUMBER_OF_SECTORS_OFFSET, SEEK_SET);
  fread(buffer, 1, NUMBER_OF_SECTORS_SIZE, file);

  return hexToInt(buffer, NUMBER_OF_SECTORS_SIZE);
}

unsigned int readSectorsPerCluster(FILE *file) {

  unsigned char buffer[SECTORS_PER_CLUSTER_SIZE];

  fseek(file, SECTORS_PER_CLUSTER_OFFSET, SEEK_SET);
  fread(buffer, 1, SECTORS_PER_CLUSTER_SIZE, file);

  return hexToInt(buffer, SECTORS_PER_CLUSTER_SIZE);
}

unsigned int readFatSectorStart(FILE *file) {

  unsigned char buffer[FAT_SECTOR_START_SIZE];

  fseek(file, FAT_SECTOR_START_OFFSET, SEEK_SET);
  fread(buffer, 1, FAT_SECTOR_START_SIZE, file);

  return hexToInt(buffer, FAT_SECTOR_START_SIZE);
}

unsigned int readRootDirCluster(FILE *file) {

  unsigned char buffer[ROOT_DIR_CLUSTER_SIZE];

  fseek(file, ROOT_DIR_CLUSTER_OFFSET, SEEK_SET);
  fread(buffer, 1, ROOT_DIR_CLUSTER_SIZE, file);

  return hexToInt(buffer, ROOT_DIR_CLUSTER_SIZE);
}

unsigned int readDataSectorStart(FILE *file) {

  unsigned char buffer[DATA_SECTOR_START_SIZE];

  fseek(file, DATA_SECTOR_START_OFFSET, SEEK_SET);
  fread(buffer, 1, DATA_SECTOR_START_SIZE, file);

  return hexToInt(buffer, DATA_SECTOR_START_SIZE);
}

void readSuperBlock(SUPERBLOCK *super) {

  FILE *file = fopen(DISK_FILE, "r");
  
  super->diskSize = readDiskSize(file);
  super->numberOfSectors = readNumberOfSectors(file);
  super->sectorsPerCluster = readSectorsPerCluster(file);
  super->fatSectorStart = readFatSectorStart(file);
  super->rootDirCluster = readRootDirCluster(file); 
  super->dataSectorStart = readDataSectorStart(file);

  fclose(file);
}

void printSuperBlock(SUPERBLOCK s) {

    printf("Disk Size: %u\nNumber of Sectors: %u\nSectors per Cluster: %u\nFAT Sector Start: %u\nRoot Directory Cluster: %u\nData Sector Start: %u\n", s.diskSize, s.numberOfSectors, s.sectorsPerCluster, s.fatSectorStart, s.rootDirCluster, s.dataSectorStart);

}


int main() {

  diskId();
  SUPERBLOCK s;
  readSuperBlock(&s);
  printSuperBlock(s);

}

