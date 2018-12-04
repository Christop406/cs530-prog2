#include <stdio.h>
#include <stdlib.h>
#include "prog2_1.h"


int main(int argc, char** argv) {
  TSAFELIST* linkedList = NULL;
  linkedList = tSafeConstruct();

  for(int i = 9 ; i >= 0 ; i--) {
    tSafeEnqueue(linkedList, i);
  }

  TSAFEDATA d = tSafeDequeue(linkedList);

  if(d.isValid) {
    printf("popped: %d\n", d.value);
  }

  tSafeEnqueue(linkedList, -100);

  tSafeDestruct(linkedList);

  printf("destructed list\n");

}

TSAFELIST* tSafeConstruct() {
  TSAFELIST* list = (TSAFELIST*) malloc(sizeof(TSAFELIST));
  list->head = NULL;
  return list;
}

void tSafeDestruct(TSAFELIST* list) {
  // for each node in the list, free all nodes,
  if(list == NULL) {
    return;
  }

  TSAFENODE* node = list->head;
  TSAFENODE* temp;
  int i = 0;

  while(node != NULL) {
    temp = node;
    node = node->next;
    free(temp);
    printf("freed entry %d\n", i);
    i++;
  }
  //then:
  free(list);
}

void tSafeEnqueue(TSAFELIST * list, mpz_t item) {
  if(list == NULL) {
    printf("list null\n");
  }
  TSAFENODE* node;
  if(list->head == NULL) {
    printf("list empty\n");
    list->head = (TSAFENODE*) malloc(sizeof(TSAFENODE));
    node = list->head;
    node->number = item;
    node->next = NULL;
    printf("added: %d\n", list->head->number);
  } else {
    int i = 0;
    TSAFENODE* toAdd = malloc(sizeof(TSAFENODE));
    node = list->head;
    toAdd->number = item;
    toAdd->next = NULL;
    // walk the list
    while(node->next != NULL) {
      printf("%d: %d\n", i, node->number);fflush(NULL);
      node = node->next;
      i++;
    }

    node->next = toAdd;
    printf("%d: %d\n", i, node->number);fflush(NULL);
    printf("added (%d): %d\n", ++i, node->next->number);
  }

}

TSAFEDATA tSafeDequeue(TSAFELIST * list) {
  TSAFEDATA data;
  if(list == NULL) {
    data.isValid = 0;
    data.value = 0;
  }
  TSAFENODE* tmpHead = list->head;
  data.isValid = 1;
  data.value = tmpHead->number;

  list->head = tmpHead->next;

  return data;
}