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

//
// funcao pra quando der open2() ou opendir2():
//
// recebe uma lista e procura o primeiro handler entre [0, máximo_da_lista - 1]
// retorna o primeiro handler vazio que achar, ou -1 se não achar nenhum
//

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
