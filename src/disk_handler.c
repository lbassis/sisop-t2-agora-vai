#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <records_list.h>
#include <string.h>

extern struct t2fs_superbloco *superblock;

void print_sector(int sector) {
  int i, j;
  int columns = 16;

  char buffer[SECTOR_SIZE];

  read_sector(sector, buffer);
  for (i = 0; i < columns; i++) {
    for (j = 0; j < SECTOR_SIZE / columns; j++) {
      //printf("%hhx\t", buffer[j + ((SECTOR_SIZE / columns) * i)]);
    }
    //printf("\n");
  }
}

void print_cluster(char *buffer) {
  int i, j;
  int size = superblock->SectorsPerCluster * SECTOR_SIZE;
  int columns = 16;

  for (i = 0; i < size / columns; i++) {
    for (j = 0; j < columns; j++) {
      ////printf("%hhx\t", buffer[j + (columns * i)]);
      //printf("%c", buffer[j + (columns * i)]);

    }
    ////printf("\n");
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

int get_record_initial_cluster(struct t2fs_record record) {
    return record.firstCluster;
}

struct t2fs_record *find_record(RECORDS_LIST *list, char *name) {

    RECORDS_LIST *aux = list;

    while (aux != NULL) {
        ////printf("comparando %s com %s\n", aux->record.name, name);
        if (strcmp(aux->generic_file.record.name, name) == 0) {
            return &(aux->generic_file.record);
        }
        aux = aux->next;
    }

    return NULL;
}

void print_record(struct t2fs_record record) {
  //printf("=========================\n\n");
  //printf("TypeVal: %hhx\n", record.TypeVal);
  //printf("name: %s\n", record.name);
  //printf("bytesFileSize: %i\n", record.bytesFileSize);
  //printf("firstCluster: %i\n\n", record.firstCluster);
}


void print_record2(struct t2fs_record record) {
  printf("=========================\n\n");
  printf("TypeVal: %hhx\n", record.TypeVal);
  printf("name: %s\n", record.name);
  printf("bytesFileSize: %i\n", record.bytesFileSize);
  printf("firstCluster: %i\n\n", record.firstCluster);
}

void read_cluster(int cluster_index, char *buffer) {
  int i = 0, j = 0, offset = 0;
  int first_sector = superblock->DataSectorStart + (cluster_index * superblock->SectorsPerCluster);
  unsigned char local_buffer[SECTOR_SIZE];

  do {
    read_sector(first_sector + i, local_buffer);
    for (j = 0; j < sizeof(local_buffer); j++) {
      buffer[j + offset] = local_buffer[j];
    }

    offset += SECTOR_SIZE; // b | era 256, ofc sector_size
    i ++;
  } while(i < superblock->SectorsPerCluster); // sectors per cluster
}

void read_all_records(int cluster_index, RECORDS_LIST **records) {
  int i = 0;
  int cluster_size = superblock->SectorsPerCluster * SECTOR_SIZE;

  int number_of_records = cluster_size / sizeof(struct t2fs_record);
  unsigned char *buffer;
  buffer = malloc(sizeof(unsigned char)*cluster_size);
  // struct t2fs_record record;
  GENERIC_FILE generic_file;

  *records = newList();

  read_cluster(cluster_index, buffer);

  for (i = 0; i < number_of_records; i++) {
    int offset = sizeof(struct t2fs_record) * i;

    generic_file.record = read_record(buffer, offset);
    // Aqui deve inserir o record na lista SE E SOMENTE SE o TypeVal dele não for 0
    if (generic_file.record.TypeVal != TYPEVAL_INVALIDO) { // era  generic_file.record.TypeVal != 0 .
      insert_record(records, generic_file);
    }
  }

  free(buffer);
  ////printf("\n\n");

  //print_records(*records);
}

int get_initial_cluster_from_path(char *path) {
    int root_cluster = superblock->RootDirCluster; // superblock->RootDirCluster; deveria ser este valor.

    RECORDS_LIST *directory;
    GENERIC_FILE current_generic_file;
    // struct t2fs_record current_record;
    char *buffer, *pathCopy;
    int current_initial_cluster = root_cluster;

    pathCopy = malloc((strlen(path)+1)*sizeof(char));
    strcpy(pathCopy, path);

    read_all_records(root_cluster, &directory);

    buffer = strtok(pathCopy, DELIM);

    while (buffer != NULL) {
         if (find_record(directory, buffer) != NULL) { // acha o record com esse nome
            current_generic_file.record = *find_record(directory, buffer);
            current_initial_cluster = get_record_initial_cluster(current_generic_file.record); // pega o inicio do cluster desse diret
            destroy_list(&directory); // limpa a lista
            read_all_records(current_initial_cluster, &directory); // recomeça a parada
            buffer = strtok(NULL, "/");
        }

        else {
            return ERROR;
        }
    }

    return current_initial_cluster;
}

void ls() {
    printf("ls no %s:\n", current_path);
    RECORDS_LIST *a;
    read_all_records(get_initial_cluster_from_path(current_path), &a);
    print_records2(a);
}

unsigned int first_empty_cluster() {

    char buffer[SECTOR_SIZE];
    int i = 0, j, k;
    int zeroes, found = 0;

    int fat_sectors = superblock->DataSectorStart - superblock->pFATSectorStart; //superblock->DataSectorStart - superblock->pFATSectorStart; // dataSectorStart - fatSectorStart  int fat_sectors = 128
    int first_sector = superblock->pFATSectorStart;//1; // superblock->pFATSectorStart; //superblock.fat_sector_start ERA 1

    int sectors_per_cluster = superblock->SectorsPerCluster;

    while (i < 1 && !found) {  // enquanto tiver setores e nao tiver achado um livre
        read_sector(first_sector+i, buffer);

        // aqui tem que ler de 4 em 4 chars pra ver se acha um só de 0s
        j = 0;
        while (j < fat_sectors/sectors_per_cluster) { // numero de valores associados a cluster
            k = 0;
            zeroes = 0; // numero de 0s encontrados
            //printf("testando o cluster %d\n", i+j);
            while (k < sectors_per_cluster) { // cada cluster tem 8 chars
                //printf("%hhx ", buffer[j*4+k]);
                if (buffer[j*sectors_per_cluster+k] == 0) {
                    zeroes++;
                }
                k++;
            }
            //printf("\n");
            if (zeroes == sectors_per_cluster) { // tudo zerado retorna o cluster que é assim
                //printf("achou 4 zeros no %d\n", i+j);
                return i+j; // substituir isso por current_sector
            }
            j++;
        }
        i++;
    }

}

char *get_filename_from_path(char *path) {
    if (strcmp(path, "/") == 0) {
        return path;
    }

    char *buffer, *pathCopy, *filename;

    pathCopy = malloc((strlen(path)+1)*sizeof(char));
    strcpy(pathCopy, path);

    buffer = strtok(pathCopy, "/");

    while(buffer != NULL) {
        filename = buffer;

        buffer = strtok(NULL, "/");
    }

    return filename;
}

char *get_father_dir_path(char *path) {
    // //printf("\n\n\n\n\n");

    if (strcmp(path, "/") == 0) {
        return path;
    }

    char *buffer, *pathCopy, *father;
    int levels = 0;

    pathCopy = malloc((strlen(path)+1)*sizeof(char));
    father = malloc((strlen(path)+1)*sizeof(char));

    strcpy(pathCopy, path);

    buffer = strtok(pathCopy, "/");

    char result[100] = "/";
    char current[100] = "/";

    // //printf("> buffer: %s\n", buffer);
    // //printf("> current: %s\n", current);
    // //printf("> previous: %s\n\n", previous);

    while(buffer != NULL) {
        strcpy(current, buffer);

        levels += 1;
        buffer = strtok(NULL, "/");

        if (buffer != NULL) {
          strcat(result, current);
          strcat(result, "/");

          strcpy(current, buffer);

        } else {
          break;
        }
    }

    if (levels <= 1) {
        return "/";
    }

    // father = result;
    strcpy(father, result);

    return father;
}

// struct t2fs_record read_record(char *buffer, int start) {
//   struct t2fs_record record;
//
//   memcpy(&record.TypeVal, buffer + start, sizeof(record.TypeVal));
//   memcpy(&record.name, buffer + start + 1, sizeof(record.name));
//   memcpy(&record.bytesFileSize, buffer + start + 56, sizeof(record.bytesFileSize));
//   memcpy(&record.firstCluster, buffer + start + 60, sizeof(record.firstCluster));
//
//   return record;
// }



int write_cluster(int cluster_index, char *buffer) {
  // faz a mágica aqui
  int i = 0, j = 0, offset = 0;
  int first_sector = superblock->DataSectorStart + (cluster_index * superblock->SectorsPerCluster);
  unsigned char sector_buffer[SECTOR_SIZE];

  do {
    for (j = 0; j < sizeof(sector_buffer); j++) {
      sector_buffer[j] = buffer[j + offset];
    }
    write_sector(first_sector + i, sector_buffer);

    offset += SECTOR_SIZE;
    i ++;
    } while(i < 4);

  return SUCESS;
}

void zero_cluster(int cluster_index) {

  int sectors_per_cluster = superblock->SectorsPerCluster;
  int i;
  char buffer[SECTOR_SIZE*sectors_per_cluster];

  for (i = 0; i < sectors_per_cluster*SECTOR_SIZE; i++)
    buffer[i] = 0;

  write_cluster(cluster_index, buffer);
}

void truncate_cluster(int cluster_index, int offset) {

  int cluster_size = SECTOR_SIZE * superblock->SectorsPerCluster; // SECTOR_SIZE * superblock->SectorsPerCluster; // 1024
  char *buffer, *cluster_content;

  buffer = malloc(sizeof(char)*SECTOR_SIZE*superblock->SectorsPerCluster);
  cluster_content = malloc(sizeof(char)*SECTOR_SIZE*superblock->SectorsPerCluster);
  //char buffer[SECTOR_SIZE * 4]; // SECTOR_SIZE * superblock->SectorsPerCluster]; // 1024
  //char cluster_content[SECTOR_SIZE * 4]; // SECTOR_SIZE * superblock->SectorsPerCluster];  // 1024

  int i = 0;

  read_cluster(cluster_index, cluster_content);

  for (i = 0; i < offset; i++) {
    buffer[i] = cluster_content[i];
  }

  for (i = offset; i < cluster_size; i++) {
    buffer[i] = 0;
  }

  write_cluster(cluster_index, buffer);
  free(buffer);
  free(cluster_content);
}

void write_record_to_buffer(char *buffer, int start, struct t2fs_record *record) {
  memcpy(buffer + start, &(record->TypeVal), sizeof(record->TypeVal));
  memcpy(buffer + start + 1, record->name, sizeof(record->name));
  memcpy(buffer + start + 56, &(record->bytesFileSize), sizeof(record->bytesFileSize));
  memcpy(buffer + start + 60, &(record->firstCluster), sizeof(record->firstCluster));
}

void init_buffer_with_zeros(char *buffer) {
  int i;
  int cluster_size = superblock->SectorsPerCluster * SECTOR_SIZE;

  for (i = 0; i < cluster_size; i++) {
    buffer[i] = 0;
  }
}

int write_list_of_records_to_cluster(RECORDS_LIST *list, int cluster_index) {
  int offset = 0, i = 0;
  int cluster_size = superblock->SectorsPerCluster * SECTOR_SIZE;

  // coloca zero em todo o buffer do cluster
  char *buffer;
  buffer = malloc(sizeof(char)*cluster_size);
  init_buffer_with_zeros(buffer);

  RECORDS_LIST *aux;
  aux = list;

  // percorre toda a lista de entradas
  // pra cada uma, adiciona suas informações no buffer do cluster
  while (aux != NULL) {
    offset = sizeof(struct t2fs_record) * i;
    write_record_to_buffer(buffer, offset, &aux->generic_file.record);

    aux = aux->next;
    i++;
  }

  // escreve as infos contidas no buffer pro disco
  write_cluster(cluster_index, buffer);

  free(buffer);

  return SUCESS;
}

int subcd (char *pathname) {

  char *current_copy = malloc(sizeof(int)*(1 + strlen(current_path)));
  strcpy(current_copy, current_path);
  free(current_path);


  DIR2 dir;
  if ((dir = opendir2(pathname)) != -1) {

    if (strcmp(pathname, "..") == 0) {
      strcpy(current_path, get_father_dir_path(pathname));
    }

    else if (strcmp(pathname, ".") == 0) {
      strcpy(current_path, current_copy);
    }

    else if (pathname[strlen(pathname)-1] != '/' ) {
      char *bar = "/";

      current_path = malloc(sizeof(char) * (1 + strlen(current_copy) + strlen(bar)));
      strcpy(current_path, current_copy);
      strcat(current_path, pathname);
      strcat(current_path, bar);
    }

    else {
      current_path = malloc(sizeof(char) * (1 + strlen(current_copy)));
      strcpy(current_path, current_copy);
      strcat(current_path, pathname);
    }

    closedir2(dir);
    return SUCESS;
  }

  else {
    strcpy(current_path, current_copy);
    return ERROR;
  }
}

// write_cluster_partially(last_cluster, buffer, buffer_pointer, relative_pointer, ammount_to_write)
int write_cluster_partially(int cluster_index, char *buffer, int buffer_pointer, int cluster_pointer, int ammount_to_write) {

  int cluster_size = superblock->SectorsPerCluster * SECTOR_SIZE;
  unsigned char *cluster;
  cluster = malloc(sizeof(unsigned char)*cluster_size);

  read_cluster(cluster_index, cluster);
  printf("====================\n");
  printf("cluster_pointer: %i\n", cluster_pointer);
  printf("buffer_pointer: %i\n", buffer_pointer);
  printf("====================\n");
  memcpy(cluster + cluster_pointer, buffer + buffer_pointer, ammount_to_write);

  write_cluster(cluster_index, cluster);
  free(cluster);
  // read_cluster(cluster_index, cluster);
  // printf("\nCLUSTER %i CONTENT:\n%s\n\n", cluster_index, cluster);

  return SUCESS;
}

// write_cluster_partially(current_cluster, buffer, buffer_pointer, relative_pointer, ammount_to_write)
