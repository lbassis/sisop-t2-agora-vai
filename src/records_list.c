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

  if (q == NULL) {
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
