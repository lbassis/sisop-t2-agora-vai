#include <records_list.h>

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

  printf("Printing list:\n");
  while (aux != NULL) {
    printf("%s\n", aux->generic_file.record.name);
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
