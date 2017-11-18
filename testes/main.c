#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <superblock.h>

// superblock.dataSectorStart é 129
#define DATA_SECTOR_START = 129

void print_sector(int sector) {
  int i, j;
  int sector_size = 256;
  int sectors_per_cluster = 16;
  
  char buffer[sector_size];
  
  read_sector(sector, buffer);
  for (i = 0; i < sectors_per_cluster; i++) {
    for (j = 0; j < sector_size / sectors_per_cluster; j++) {
      printf("%hhx\t", buffer[j + ((sector_size / sectors_per_cluster) * i)]);
    }
    printf("\n");
  }
}

struct t2fs_record read_record(char *buffer) {
  struct t2fs_record record;
  // strcpy(record.name, "oyo");
  memcpy(&record.TypeVal, buffer, sizeof(BYTE));
  memcpy(&record.name, buffer + 1, sizeof(55));
  memcpy(&record.bytesFileSize, buffer + 56, sizeof(DWORD));
  memcpy(&record.firstCluster, buffer + 60, sizeof(DWORD));
  
  return record;
}

void print_record(struct t2fs_record record) {
  printf("=========================\n\n");
  printf("TypeVal: %hhx\n", record.TypeVal);
  printf("name: %s\n", record.name);
  printf("bytesFileSize: %i\n", record.bytesFileSize);
  printf("firstCluster: %i\n\n", record.firstCluster);
}

void read_cluster(int index) {
}

int main() {
  int first_sector = 137;
  unsigned char buffer[256];
  struct t2fs_record record;
  
  read_sector(first_sector, buffer);
  record = read_record(buffer);
  print_record(record);
  
  read_sector(first_sector + 1, buffer);
  record = read_record(buffer);
  print_record(record);
  
  read_sector(first_sector + 2, buffer);
  record = read_record(buffer);
  print_record(record);
  
  printf("\nSente soh: o root começa no setor 137. Ele tem duas entradas:\n \
   \n'.' -> referência pro próprio diretório\n \
   \n'dir1 alguma coisa' -> provavelmente um diretório\n\n");
  
  return 0;
}
