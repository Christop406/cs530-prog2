#include <stdio.h>
#include <stdlib.h>
#include "prog2_1.h"
#include <pthread.h>

TSAFELIST* tSafeConstruct() {
    TSAFELIST* list = (TSAFELIST*) malloc(sizeof(TSAFELIST));
    list->head = NULL;
    list->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(list->mutex, NULL);
    return list;
}

void tSafeDestruct(TSAFELIST* list) {
    pthread_mutex_lock(list->mutex);
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
        //printf("freed entry %d\n", i);
        i++;
    }
    pthread_mutex_unlock(list->mutex);
    //then:
    free(list->mutex);
    free(list);
}

void tSafeEnqueue(TSAFELIST * list, mpz_t item) {
    pthread_mutex_lock(list->mutex);
//    gmp_printf ("item value: %Zd\n", item);
    if(list == NULL) {
        //printf("list null\n");
        // don't do things
        return;
    }
    TSAFENODE* node;
    if(list->head == NULL) {
        //printf("list empty\n");
        list->head = (TSAFENODE*) malloc(sizeof(TSAFENODE));
        node = list->head;
        mpz_init_set(node->number, item);
//        gmp_printf ("node-num value: %Zd\n", node->number);
        node->next = NULL;
        //printf("added: %d\n", list->head->number);
    } else {
        int i = 0;
        TSAFENODE* toAdd = malloc(sizeof(TSAFENODE));
        node = list->head;
        mpz_init_set(toAdd->number, item);
//        gmp_printf ("toAdd value: %Zd\n", toAdd->number);
        toAdd->next = NULL;
        // walk the list
        while(node->next != NULL) {
            //printf("%d: %d\n", i, node->number);fflush(NULL);
            node = node->next;
            i++;
        }
        
        node->next = toAdd;
        //printf("%d: %d\n", i, node->number);fflush(NULL);
        //printf("added (%d): %d\n", ++i, node->next->number);
    }
    pthread_mutex_unlock(list->mutex);
}

TSAFEDATA tSafeDequeue(TSAFELIST * list) {
    pthread_mutex_lock(list->mutex);
    TSAFEDATA data;
    if(list == NULL) {
        data.isValid = 0;
        //data.value = 0;
    }
    TSAFENODE* tmpHead = list->head;
    data.isValid = 1;
    mpz_init_set(data.value, tmpHead->number);
    
//    gmp_printf ("data.value: %Zd\n", data.value);
    
    list->head = tmpHead->next;
    pthread_mutex_unlock(list->mutex);
    return data;
}
