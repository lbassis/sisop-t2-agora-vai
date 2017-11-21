#include <records_list.h>

RECORDS_LIST *newList() {
  return NULL;
}

void insert_record(RECORDS_LIST **q, struct t2fs_record record) {

  RECORDS_LIST *aux;
  RECORDS_LIST *new;

  aux = *q;

  new = malloc(sizeof(RECORDS_LIST));
  new->record = record;
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

  printf("Printing list:\n");
  while (aux != NULL) {
    printf("%s\n", aux->record.name);
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

struct t2fs_record *get_record_at_index(RECORDS_LIST *q, int index) {
  
  RECORDS_LIST *aux;
  aux = q;
  int i = 0, found = 0;

  while (aux != NULL) {
    if (i == index) {
      return &(aux->record);
    }
    
    i ++;
    aux = aux->next;
  }
  
  return NULL;
}

int remove_record_at_index(RECORDS_LIST **q, int index) {
  
  RECORDS_LIST *aux, *next, *prev;
  aux = *q;
  int i = 0, found = 0;
  
  prev = aux;
  
  while (aux != NULL) {
    if (i == index) {
      if (prev == aux) {
        *q = aux->next;
      } else {
        prev->next = aux->next;
      }
      
      free(aux);
      return 0;
    }
    
    i ++;
    prev = aux;
    aux = aux->next;
  }
  
  return -1;
}
