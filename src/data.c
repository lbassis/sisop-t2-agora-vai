#include <stdio.h>

typedef struct LIST {
  char element;
  struct LIST *next;
} LIST;

LIST *newList() {
  return NULL;
}

void insert(LIST **q, char element) { 

  LIST *aux;
  LIST *new;
  
  aux = *q;

  new = malloc(sizeof(LIST)); 
  new->elementt = element;


  if (q == NULL) {
    *q = new;
    (*q)->next = NULL;
  }
  
  else {
    
    while (aux->next != NULL) { 
      aux = aux->next;
    }
    aux->next = new; 
  }
}

void printList(LIST *q) {

  LIST *aux;
  aux = q;

  printf("Printing list:\n");
  while (aux != NULL) {
    printf("%d\n", aux->element);
    aux = aux->next;
  }

}
