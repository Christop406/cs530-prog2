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
        i++;
    }
    pthread_mutex_unlock(list->mutex);
    pthread_mutex_destroy(list->mutex);
    //then:
    free(list->mutex);
    free(list);
}

void tSafeEnqueue(TSAFELIST * list, mpz_t item) {
    pthread_mutex_lock(list->mutex);
    if(list == NULL) {
        return;
    }
    TSAFENODE* node;
    if(list->head == NULL) {
        list->head = (TSAFENODE*) malloc(sizeof(TSAFENODE));
        node = list->head;
        mpz_init_set(node->number, item);
        node->next = NULL;
    } else {
        int i = 0;
        TSAFENODE* toAdd = malloc(sizeof(TSAFENODE));
        node = list->head;
        mpz_init_set(toAdd->number, item);
        toAdd->next = NULL;
        // walk the list
        while(node->next != NULL) {
            node = node->next;
            i++;
        }
        
        node->next = toAdd;
    }
    pthread_mutex_unlock(list->mutex);
}

TSAFEDATA tSafeDequeue(TSAFELIST * list) {
    pthread_mutex_lock(list->mutex);
    TSAFEDATA data;
    if(list == NULL || list->head == NULL) {
        data.isValid = 0;
        pthread_mutex_unlock(list->mutex);
        return data;
    }
    TSAFENODE* tmpHead = list->head;
    data.isValid = 1;
    mpz_init_set(data.value, tmpHead->number);
    
    
    list->head = tmpHead->next;
    pthread_mutex_unlock(list->mutex);
    return data;
}
