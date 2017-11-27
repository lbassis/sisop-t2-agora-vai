#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <superblock.h>

#include <apidisk.h>
#include <t2fs.h>


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


// void diskId() { // só printa mesmo
//
//   char id[ID_SIZE+1];
//   FILE *file = fopen(DISK_FILE, "r");
//
//   fseek(file, 0, SEEK_SET);
//
//   fread(id, 1, ID_SIZE, file);
//   id[ID_SIZE] = (char) 0; // marca de final da string
//   printf("ID: %s\n", id);
//
//   fclose(file);
// }

unsigned int readSuperBlockSize(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*SUPER_BLOCK_SIZE);
  memcpy(mini_buffer, buffer+SUPER_BLOCK_SIZE_OFFSET, SUPER_BLOCK_SIZE);
  int result = hexToInt(mini_buffer, SUPER_BLOCK_SIZE);
  free(mini_buffer);

  return result;

}

unsigned int readDiskSize(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*DISK_SIZE);
  memcpy(mini_buffer, buffer+DISK_SIZE_OFFSET, DISK_SIZE);
  int result = hexToInt(mini_buffer, DISK_SIZE);
  free(mini_buffer);

  return result;

}

unsigned int readNumberOfSectors(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*NUMBER_OF_SECTORS_SIZE);
  memcpy(mini_buffer, buffer+NUMBER_OF_SECTORS_OFFSET, NUMBER_OF_SECTORS_SIZE);
  int result = hexToInt(mini_buffer, NUMBER_OF_SECTORS_SIZE);
  free(mini_buffer);

  return result;
}

unsigned int readSectorsPerCluster(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*SECTORS_PER_CLUSTER_SIZE);
  memcpy(mini_buffer, buffer+SECTORS_PER_CLUSTER_OFFSET, SECTORS_PER_CLUSTER_SIZE);
  int result = hexToInt(mini_buffer, SECTORS_PER_CLUSTER_SIZE);
  free(mini_buffer);

  return result;
}

unsigned int readFatSectorStart(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*FAT_SECTOR_START_SIZE);
  memcpy(mini_buffer, buffer+FAT_SECTOR_START_OFFSET, FAT_SECTOR_START_SIZE);
  int result = hexToInt(mini_buffer, FAT_SECTOR_START_SIZE);
  free(mini_buffer);

  return result;}

unsigned int readRootDirCluster(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*ROOT_DIR_CLUSTER_SIZE);
  memcpy(mini_buffer, buffer+ROOT_DIR_CLUSTER_OFFSET, ROOT_DIR_CLUSTER_SIZE);
  int result = hexToInt(mini_buffer, ROOT_DIR_CLUSTER_SIZE);
  free(mini_buffer);

  return result;
}

unsigned int readDataSectorStart(char *buffer) {

  char *mini_buffer;
  mini_buffer = malloc(sizeof(char)*DATA_SECTOR_START_SIZE);
  memcpy(mini_buffer, buffer+DATA_SECTOR_START_OFFSET, DATA_SECTOR_START_SIZE);
  int result = hexToInt(mini_buffer, DATA_SECTOR_START_SIZE);
  free(mini_buffer);

  return result;
}

void readSuperBlock(struct t2fs_superbloco *superblock) {

  //FILE *file = fopen(DISK_FILE, "r");

  char buffer[1024]; // bem maior só por garantia
  read_sector(0, buffer);


  superblock->SuperBlockSize = readSuperBlockSize(buffer);
  superblock->DiskSize = readDiskSize(buffer);
  superblock->NofSectors = readNumberOfSectors(buffer);
  superblock->SectorsPerCluster = readSectorsPerCluster(buffer);
  superblock->pFATSectorStart = readFatSectorStart(buffer);
  superblock->RootDirCluster = readRootDirCluster(buffer);
  superblock->DataSectorStart = readDataSectorStart(buffer);
  printSuperBlock(*superblock);
  //
  // fclose(file);
}

void printSuperBlock(struct t2fs_superbloco superblock) {

    printf("Disk Size: %u\nNumber of Sectors: %u\nSectors per Cluster: %u\nFAT Sector Start: %u\nRoot Directory Cluster: %u\nData Sector Start: %u\n", superblock.DiskSize, superblock.NofSectors, superblock.SectorsPerCluster, superblock.pFATSectorStart, superblock.RootDirCluster, superblock.DataSectorStart);

}

// int main() {
//
//   diskId();
//   SUPERBLOCK s;
//   readSuperBlock();
//
//   int dataStart = superblock.dataSectorStart * SECTOR_SIZE;
//   int rootFatEntry = dataStart + superblock.rootDirCluster * superblock.sectorsPerCluster * SECTOR_SIZE;
//
//   printf("%d\n", rootFatEntry);
//
// }
