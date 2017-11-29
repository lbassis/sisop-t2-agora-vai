#include <records_list.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <disk_handler.h>

extern struct t2fs_superbloco *superblock;

RECORDS_LIST *newList() {
  return NULL;
}

void insert_record(RECORDS_LIST **q, GENERIC_FILE generic_file) {

  RECORDS_LIST *aux;
  RECORDS_LIST *new;

  aux = *q;

  new = malloc(sizeof(RECORDS_LIST));
  new->generic_file = generic_file;
  new->next = NULL;


  if (*q == NULL) {
    *q = new;
  }

  else {
    while (aux->next != NULL) {
      aux = aux->next;
    }
    aux->next = new;
  }
}

void print_records(RECORDS_LIST *q) {

  RECORDS_LIST *aux;
  aux = q;

  //printf("Printing list:\n\n");
  while (aux != NULL) {
    print_record(aux->generic_file.record);
    //printf("%s no cluster %d\n", aux->generic_file.record.name, aux->generic_file.record.firstCluster);
    aux = aux->next;
  }
}


void destroy_list(RECORDS_LIST **q) {

    RECORDS_LIST *aux, *next;
    aux = *q;

    while (aux != NULL) {
        next = aux->next;
        free(aux);
        aux = next;
    }
    *q = NULL;
}

int list_length(RECORDS_LIST *q) {

  RECORDS_LIST *aux;
  aux = q;
  int len = 0;

  while (aux != NULL) {
    len ++;
    aux = aux->next;
  }

  return len;
}

GENERIC_FILE *get_record_at_index(RECORDS_LIST *q, int index) {

  RECORDS_LIST *aux;
  aux = q;
  int i = 0, found = 0;

  while (aux != NULL) {
    if (i == index) {
      // //printf("achou o %s\n", aux->generic_file.record.name);
      return &(aux->generic_file);
    }

    i ++;
    aux = aux->next;
  }

  return NULL;
}

GENERIC_FILE *get_record_at_filename(RECORDS_LIST *q, char *name) {

  RECORDS_LIST *aux;
  aux = q;
  int i = 0, found = 0;

  while (aux != NULL) {
    if (strcmp(name, aux->generic_file.record.name) == 0) {
      return &(aux->generic_file);
    }

    i ++;
    aux = aux->next;
  }

  return NULL;
}

GENERIC_FILE *get_record_with_handle(RECORDS_LIST *q, int handle) {

  RECORDS_LIST *aux;
  aux = q;
  int i = 0, found = 0;

  while (aux != NULL) {
    if (handle == aux->generic_file.handler) {
      return &(aux->generic_file);
    }

    i ++;
    aux = aux->next;
  }

  return NULL;
}

int remove_record_at_index(RECORDS_LIST **q, int handler_to_remove) {

  RECORDS_LIST *aux, *next, *prev;
  aux = *q;

  prev = aux;

  while (aux != NULL) {
     if (aux->generic_file.handler == handler_to_remove) {

         //printf("a remove_record_at_index ta removendo o %s", aux->generic_file.record.name);
         if (prev == aux) {
           *q = aux->next;
         } else {
           prev->next = aux->next;
         }
         free(aux);
         return 0;
     }

    prev = aux;
    aux = aux->next;
  }

  return -1;
}

//
// funcao pra quando der open2() ou opendir2():
//
// recebe uma lista e procura o primeiro handler entre [0, máximo_da_lista - 1]
// retorna o primeiro handler vazio que achar, ou -1 se não achar nenhum
//

int remove_record_at_filename(RECORDS_LIST **q, char *name) {


  //printf("entrou na remove_record_at_filename com name %s\n", name);
  RECORDS_LIST *aux, *next, *prev;
  aux = *q;

  prev = aux;

  while (aux != NULL) {
     if (strcmp(aux->generic_file.record.name, name) == 0) {
         if (prev == aux) { // se era o primeiro
           //*q = aux->next; // só pula o ponteiro pro proximo
           *q = (*q)->next;

         } else {
           //printf("o proximo do %s agora é o %s\n", prev->generic_file.record.name, aux->next->generic_file.record.name);
           prev->next = aux->next;
         }
         free(aux);

         //printf("a lista ficou assim:::\n");
         print_records(*q);
         return 0;
     }

    prev = aux;
    aux = aux->next;
  }

  return -1;
}


int get_fisrt_handler_available(RECORDS_LIST *list, int list_max_items) {
    RECORDS_LIST *aux;
    int i, found_handler;

    // pra cada valor de handler possível
    for (i = 0; i < list_max_items - 1; i++) {

        // bota o aux no início da lista e seta flag pra 0
        aux = list;
        found_handler = 0;

        // percorre a lista vendo se o handler i já foi usado
        while (aux != NULL) {
            // se este handler estiver em uso pelo item atual da lista, seta uma flag
            if (aux->generic_file.handler == i) {
                found_handler = 1;
                break;
            }
          aux = aux->next;
        }

        // se percorreu toda a lista e não achou o handler em nenhum item da lista, retorna ele
        if (!found_handler) {
            return i;
        }
    }

    // se chegar aqui é pq não tem jeito, não há nenhum handler disponível
    return -1;
}

struct t2fs_record *create_record(BYTE typeVal, char *name, DWORD bytesFileSize, DWORD firstCluster) {
  struct t2fs_record *record = malloc(sizeof(struct t2fs_record));

  record->TypeVal = typeVal; // diretorio
  strcpy(record->name, name);
  record->bytesFileSize = bytesFileSize;
  record->firstCluster = firstCluster;

  return record;
}

GENERIC_FILE *create_generic_new_file(struct t2fs_record *record, int handler, int pointer) {
  GENERIC_FILE *file = malloc(sizeof(GENERIC_FILE));

  file->record = *record;
  file->handler = handler; // bota -1 pq nesse caso nao faz sentido usar o handler
  file->pointer = pointer;

  return file;
}

int create_default_records_in_directory(RECORDS_LIST **list, int self_first_cluster, int father_first_cluster) {
  // cria record do . e do ..
  struct t2fs_record *dot_record = (struct t2fs_record *) create_record(2, ".", 0, self_first_cluster);
  struct t2fs_record *dot_dot_record = (struct t2fs_record *) create_record(2, "..", 0, father_first_cluster);

  // cria generic files dele também
  GENERIC_FILE *dot_file = (GENERIC_FILE *) create_generic_new_file(dot_record, -1, 0);
  GENERIC_FILE *dot_dot_file = (GENERIC_FILE *) create_generic_new_file(dot_dot_record, -1, 0);

  //insere eles na lista
  insert_record(list, *dot_file);
  insert_record(list, *dot_dot_file);
}

int delete_all_records(RECORDS_LIST **q) {
  int i = 0;
  int length = list_length(*q);
  RECORDS_LIST *aux = *q;
  RECORDS_LIST *next = aux;



  char *name;
  int type;

  while (aux != NULL) {
    if (strcmp(aux->generic_file.record.name, "..") != 0) {

      if (aux->generic_file.record.TypeVal == 1)
        delete2(aux->generic_file.record.name);
      else
        rmdir2(aux->generic_file.record.name);

      next = aux->next;
      remove_record_at_filename(q, aux->generic_file.record.name);
    }

    else {
      next = aux->next;
    }

    aux = next;
  }

  print_records(*q);
}

int find_last_cluster(GENERIC_FILE *file) {
  int next, current = file->record.firstCluster;
  next = current;

  do {
    current = next;
    next = read_fat_entry(current);

  } while(next != 0 && next != -1 && next != -2);

  return current;
}

// mapeia o ponteiro absoluto (nro de bytes independente de quantos clusters tem) parada
// valor relativo dentro do último cluster

// se pointer = 0 => 0
// se pointer = 1023 => 1023
// se pointer = 1025 => 1
// se pointer = 2047 => 1023
// se pointer = 2048 => 0
int calculate_relative_pointer(GENERIC_FILE *file) {
  int pointer = file->pointer;

  int bytes_per_cluster = SECTOR_SIZE * superblock->SectorsPerCluster;
  while (pointer >= bytes_per_cluster) {
    pointer -= bytes_per_cluster;
  }

  return pointer;
}

int calculate_bytes_available_in_last_cluster(GENERIC_FILE *file) {
  int occupied = file->record.bytesFileSize;

  int bytes_per_cluster = SECTOR_SIZE * superblock->SectorsPerCluster;

  while (occupied > bytes_per_cluster) {
    occupied -= bytes_per_cluster;
  }

  printf("====> livres: %i\n", bytes_per_cluster - occupied);

  return bytes_per_cluster - occupied;
}

int find_cluster_from_pointer(GENERIC_FILE *file) {
  int pointer = file->pointer;
  int cluster = file->record.firstCluster;

  int bytes_per_cluster = SECTOR_SIZE * superblock->SectorsPerCluster;

  // enquanto o valor do ponteiro indicar que tem mais de 1 cluster a frente
  while (pointer > bytes_per_cluster) {
    // diminui 1024
    pointer -= bytes_per_cluster;

    // e pega o próximo cluster
    cluster = read_fat_entry(cluster);
  }

  return cluster;
}

int update_bytesFileSize(GENERIC_FILE *file, RECORDS_LIST *list) {
  RECORDS_LIST *aux;
  aux = list;

  while (aux != NULL) {
    // achou o elemento
    if (strcmp(aux->generic_file.record.name, file->record.name) == 0) {
      // substitui o bytesFileSize
      aux->generic_file.record.bytesFileSize = file->record.bytesFileSize;
      return 0;
    }
    aux = aux->next;
  }

  // se chegou aqui é pq nao achou.
  return -1;
}
