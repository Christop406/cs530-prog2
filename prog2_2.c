#include <stdio.h>
#include <stdlib.h>
#include "prog2_1.c"
#include <gmp.h>
#include <stdarg.h>


mpz_t globalCounter;
int foundCount = 0;
pthread_mutex_t counterguard = PTHREAD_MUTEX_INITIALIZER;
TSAFELIST *numberList;

void getNextNum(mpz_t * num) {
    pthread_mutex_lock(&counterguard);
    mpz_add_ui(globalCounter, globalCounter, 2);
    //mpz_set(*num, globalCounter);
    pthread_mutex_unlock(&counterguard);
}

void * generatePrimes(void * args) {
    int prob = 0;
    int i = 0;
    mpz_t tempCounter;
    pthread_mutex_lock(&counterguard);
    mpz_init_set(tempCounter, globalCounter);
    pthread_mutex_unlock(&counterguard);
    while(1) {
        pthread_mutex_lock(&counterguard);
        if(foundCount == 10) {
            pthread_mutex_unlock(&counterguard);
            return NULL;
        }
        pthread_mutex_unlock(&counterguard);
        getNextNum(&tempCounter);
        prob = mpz_probab_prime_p(tempCounter, 100000);
        if(prob == 2) {
            tSafeEnqueue(numberList, tempCounter);
        }
    }
}

int main(int argc, char** argv) {
    numberList = tSafeConstruct();
    mpz_init_set_ui(globalCounter, 1);
    tSafeDestruct(numberList);
}
