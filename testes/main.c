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

void print_buffer_formatted(char *buffer) {
  int i, j;
  int sector_size = 1024;
  int sectors_per_cluster = 16;

  for (i = 0; i < sector_size / sectors_per_cluster; i++) {
    for (j = 0; j < sectors_per_cluster; j++) {
      printf("%hhx\t", buffer[j + (sectors_per_cluster * i)]);
    }
    printf("\n");
  }
}

struct t2fs_record read_record(char *buffer, int start) {
  struct t2fs_record record;
  
  memcpy(&record.TypeVal, buffer + start, sizeof(record.TypeVal));
  memcpy(&record.name, buffer + start + 1, sizeof(record.name));
  memcpy(&record.bytesFileSize, buffer + start + 56, sizeof(record.bytesFileSize));
  memcpy(&record.firstCluster, buffer + start + 60, sizeof(record.firstCluster));
  
  return record;
}

void print_record(struct t2fs_record record) {
  printf("=========================\n\n");
  printf("TypeVal: %hhx\n", record.TypeVal);
  printf("name: %s\n", record.name);
  printf("bytesFileSize: %i\n", record.bytesFileSize);
  printf("firstCluster: %i\n", record.firstCluster);
}

void read_cluster(int cluster_index, char *buffer) {
  // mais tarde dá pra usar o atributo DataSectorStart do superbloco
  int dataSectorStart = 129;
  int sectorsPerCluster = 4;
  
  int i = 0, j = 0, offset = 0;
  int first_sector = dataSectorStart + (cluster_index * sectorsPerCluster);
  unsigned char local_buffer[256];
  
  do {
    read_sector(first_sector + i, local_buffer);
    for (j = 0; j < sizeof(local_buffer); j++) {
      buffer[j + offset] = local_buffer[j];
    }
  
    offset += 256;
    i ++;
  } while(i < 4);
}

// LUCAS_PAU_NO_CU_DE_ASSIS
// essa porra tem que retornar uma lista com os records (ou receber um ponteiro e ir adicionando nele)
void read_all_records(int cluster_index) {
  int i = 0;
  int number_of_records = 1024 / sizeof(struct t2fs_record);
  unsigned char buffer[256 * 4];
  struct t2fs_record record;
  
  read_cluster(cluster_index, buffer);
  
  for (i = 0; i < number_of_records; i++) {
    int offset = sizeof(struct t2fs_record) * i;
    
    record = read_record(buffer, offset);
    // Aqui deve inserir o record na lista SE E SOMENTE SE o TypeVal dele não for 0
    print_record(record);
    
    printf("\n\n");
  }
}

int main() {
  
  // struct t2fs_record []
  read_all_records(2);
  
  
  // unsigned char buffer[1024];
  // // struct t2fs_record record;
  //
  // read_cluster(2, buffer);
  // print_buffer_formatted(buffer);
  
  
  // SUPERBLOCK s;
  //
  // diskId();
  // readSuperBlock(&s);
  // printSuperBlock(s);
  
  
  // int first_sector = 137;
  //
  // read_sector(first_sector, buffer);
  // record = read_record(buffer);
  // print_record(record);
  //
  // read_sector(first_sector + 1, buffer);
  // record = read_record(buffer);
  // print_record(record);
  //
  // read_sector(first_sector + 2, buffer);
  // record = read_record(buffer);
  // print_record(record);
  //
  // printf("\nSente soh: o root começa no setor 137. Ele tem duas entradas:\n \
  //  \n'.' -> referência pro próprio diretório\n \
  //  \n'dir1 alguma coisa' -> provavelmente um diretório\n\n");
  
  return 0;
}
