#include <stdio.h>
#include <apidisk.h>
#include <t2fs.h>
#include <disk_handler.h>
#include <records_list.h>
#include <string.h>



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

int get_record_initial_cluster(struct t2fs_record record) {
    return record.firstCluster;
}

struct t2fs_record *find_record(RECORDS_LIST *list, char *name) {

    RECORDS_LIST *aux = list;

    while (aux != NULL) {
        //printf("comparando %s com %s\n", aux->record.name, name);
        if (strcmp(aux->generic_file.record.name, name) == 0) {
            return &(aux->generic_file.record);
        }
        aux = aux->next;
    }

    return NULL;
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
  
  printf("before DO\n");

  do {
    printf("will read sector %i\n", first_sector + i);
    read_sector(first_sector + i, local_buffer);
    for (j = 0; j < sizeof(local_buffer); j++) {
      buffer[j + offset] = local_buffer[j];
    }
    
    offset += 256; // b
    i ++;
    } while(i < 4); // sectors per cluster
}

void read_all_records(int cluster_index, RECORDS_LIST **records) {
  int i = 0;
  int number_of_records = CLUSTER_SIZE / sizeof(struct t2fs_record);
  unsigned char buffer[CLUSTER_SIZE];
  // struct t2fs_record record;
  GENERIC_FILE generic_file;
  
  printf("dentro da read_all_records\n");
  *records = newList();
  
  printf("vai ler o cluster %i\n", cluster_index);
  read_cluster(cluster_index, buffer);
  
  for (i = 0; i < number_of_records; i++) {
    int offset = sizeof(struct t2fs_record) * i;
    
    generic_file.record = read_record(buffer, offset);
    // Aqui deve inserir o record na lista SE E SOMENTE SE o TypeVal dele não for 0
    if (generic_file.record.TypeVal != 0) {
      printf("achei o arquivo %s\n", generic_file.record.name);
      insert_record(records, generic_file);
    }
  }
  //printf("\n\n");

  //print_records(*records);
}

int get_initial_cluster_from_path(char *path) {
    printf("entrou na get_initial_cluster_from_path\n");
    int root_cluster = 2; // TEM QUE ARRUMAR ///////////////////////////////////////

    RECORDS_LIST *directory;
    GENERIC_FILE current_generic_file;
    // struct t2fs_record current_record;
    char *buffer, *pathCopy;
    int current_initial_cluster = root_cluster;

    pathCopy = malloc(sizeof(strlen(path)));
    strcpy(pathCopy, path);

    printf("antes de read_all_records\n");

    read_all_records(root_cluster, &directory);
    
    printf("antes do strtok\n");
    buffer = strtok(pathCopy, DELIM);

    printf("logo antes do while\n");

    while (buffer != NULL) {
         if (find_record(directory, buffer) != NULL) { // acha o record com esse nome
            current_generic_file.record = *find_record(directory, buffer);
            current_initial_cluster = get_record_initial_cluster(current_generic_file.record); // pega o inicio do cluster desse diret
            destroy_list(&directory); // limpa a lista
            read_all_records(current_initial_cluster, &directory); // recomeça a parada
            buffer = strtok(NULL, "/");
        }

        else {
            return -1;
        }
    }

    // se saiu do while numa boa é pq achou tudo

    // printf("\n\nresultado do ls em %s \n", current_path);
    // RECORDS_LIST *testano;
    // read_all_records(current_initial_cluster, &testano);
    // print_records(testano);
    
    return current_initial_cluster;
}

void ls() {
    RECORDS_LIST *a;
    read_all_records(get_initial_cluster_from_path(current_path), &a);
}

unsigned int first_empty_cluster() {

    char buffer[SECTOR_SIZE];
    int i = 0, j, k;
    int zeroes, found = 0;

    int fat_sectors = 128; //dataSectorStart - fatSectorStart
    int first_sector = 1; //superblock.fat_sector_start

    while (i < 1 && !found) {  // enquanto tiver setores e nao tiver achado um livre
        read_sector(first_sector+i, buffer);

        // aqui tem que ler de 4 em 4 chars pra ver se acha um só de 0s
        j = 0;
        while (j < fat_sectors/4) { // numero de valores associados a cluster
            k = 0;
            zeroes = 0; // numero de 0s encontrados
            printf("testando o cluster %d\n", i+j);
            while (k < 4) { // cada cluster tem 8 chars
                printf("%hhx ", buffer[j*4+k]);
                if (buffer[j*4+k] == 0) {
                    zeroes++;
                }
                k++;
            }
            printf("\n");
            if (zeroes == 4) { // tudo zerado retorna o cluster que é assim
                printf("achou 4 zeros no %d\n", i+j);
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

    pathCopy = malloc(sizeof(strlen(path)));
    strcpy(pathCopy, path);

    buffer = strtok(pathCopy, "/");

    while(buffer != NULL) {
        filename = buffer;

        printf("%s\n", buffer);
        buffer = strtok(NULL, "/");
    }

    return filename;
}

char *get_father_dir_path(char *path) {
    // printf("\n\n\n\n\n");
    
    if (strcmp(path, "/") == 0) {
        return path;
    }

    char *buffer, *pathCopy, *father;
    int levels = 0;

    pathCopy = malloc(sizeof(strlen(path)));
    father = malloc(sizeof(strlen(path)));

    strcpy(pathCopy, path);
    
    buffer = strtok(pathCopy, "/");

    char result[100] = "/";
    char current[100] = "/";
    
    // printf("> buffer: %s\n", buffer);
    // printf("> current: %s\n", current);
    // printf("> previous: %s\n\n", previous);
    
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
