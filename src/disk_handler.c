#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <records_list.h>

void print_sector(int sector) {
  int i, j;
  int columns = 16;

  char buffer[SECTOR_SIZE];

  read_sector(sector, buffer);
  for (i = 0; i < columns; i++) {
    for (j = 0; j < SECTOR_SIZE / columns; j++) {
      printf("%hhx\t", buffer[j + ((SECTOR_SIZE / columns) * i)]);
    }
    printf("\n");
  }
}

void print_cluster(char *buffer) {
  int i, j;
  int size = CLUSTER_SIZE;
  int columns = 16;

  for (i = 0; i < size / columns; i++) {
    for (j = 0; j < columns; j++) {
      printf("%hhx\t", buffer[j + (columns * i)]);
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
  int i = 0, j = 0, offset = 0;
  int first_sector = DATA_SECTOR_START + (cluster_index * SECTORS_PER_CLUSTER);
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
  int number_of_records = CLUSTER_SIZE / sizeof(struct t2fs_record);
  unsigned char buffer[CLUSTER_SIZE];
  struct t2fs_record record;

  printf("aodjfsf\n");
  //RECORDS_LIST *records = newList2();
  printf("auhdfuaifds\n");

  read_cluster(cluster_index, buffer);

  for (i = 0; i < number_of_records; i++) {
    int offset = sizeof(struct t2fs_record) * i;

    record = read_record(buffer, offset);
    // Aqui deve inserir o record na lista SE E SOMENTE SE o TypeVal dele não for 0
    if (record.TypeVal != 0) {
        printf("vai inserir\n");
        //insert_record2(&records, record);
      // print_record(record);

      if (record.TypeVal == 2) {
        if (strcmp(record.name, ".") != 0 && strcmp(record.name, "..") != 0) {
          print_record(record);

          unsigned char dir_buffer[256 * 4];

          read_all_records(record.firstCluster);
        }
      }

      if (record.TypeVal == 1) {
        printf("\n==== Conteúdo do arquivo \"%s\" ====\n", record.name);

        unsigned char file_buffer[256 * 4];

        read_cluster(record.firstCluster, file_buffer);
        printf("%s\n\n", file_buffer);

        if (record.bytesFileSize > CLUSTER_SIZE) {
          // este 8 abaixo deve ser lido da fat pra saber onde está o proximo cluster do arquivo
          read_cluster(8, file_buffer);
          printf("%s\n\n", file_buffer);

          // int current_size = record.bytesFileSize;
          // while(current_size > CLUSTER_SIZE) {
          //   current_size = current_size - CLUSTER_SIZE;
          //
          //   int next_cluster = read_fat_at_position();
          //   read_cluster(next_cluster, file_buffer);
          //   printf("%s\n\n", file_buffer);
          // }
        }
      }
    }
  }
  printf("\n\n");

  printf("lista????\n");
  //print_records(records);
}
