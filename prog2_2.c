#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    mpz_set(*num, globalCounter);
    pthread_mutex_unlock(&counterguard);
}

void * generatePrimes(void * args) {
    int prob = 0;
    int* num_primes = (int*) args;
    mpz_t tempCounter;
    
    pthread_mutex_lock(&counterguard);
    mpz_init_set(tempCounter, globalCounter);
    pthread_mutex_unlock(&counterguard);
    
    while(1) {
        pthread_mutex_lock(&counterguard);
        if(foundCount == *num_primes) {
            pthread_mutex_unlock(&counterguard);
            return NULL;
        }
        
        prob = mpz_probab_prime_p(tempCounter, 100000);
        
        if(prob > 0) {
            tSafeEnqueue(numberList, tempCounter);
            foundCount++;
        }
        
        pthread_mutex_unlock(&counterguard);
        
        getNextNum(&tempCounter);
        
    }
}

int main(int argc, char** argv) {
    printf("Assignment #2-2, Chris Gilardi, cgilardi97@gmail.com\n");
    pthread_t t1, t2, t3, t4;
    TSAFEDATA data;
    int printed = 0;
    int bit_length;
    int num_primes;
    
    if(argc < 3 || argc > 3) {
        fprintf(stderr, "prog2: Wrong number of arguments.\n");
        fprintf(stderr, "prog2: usage: ./prog2_2 <k: number of primes> <b: bit length>\n");
        return -1;
    }
    
    num_primes = atoi(argv[1]);
    bit_length = atoi(argv[2]);
    
    printf("Key #: %d, Bit-length: %d, Checks/num: %d, Threads: %d\n", num_primes, bit_length, 100000, 4);
    
    numberList = tSafeConstruct();
    mpz_init(globalCounter);
    mpz_ui_pow_ui(globalCounter, 2, bit_length);
    mpz_add_ui(globalCounter, globalCounter, 1);
    pthread_create(&t1, NULL, generatePrimes, &num_primes);
    pthread_create(&t2, NULL, generatePrimes, &num_primes);
    pthread_create(&t3, NULL, generatePrimes, &num_primes);
    pthread_create(&t4, NULL, generatePrimes, &num_primes);
    
//    printf("here3\n");
    
    while(printed < 10) {
//        printf("here4\n");
        data = tSafeDequeue(numberList);
        //gmp_printf ("(%Zd) ", globalCounter);
//        printf("here5\n");
        if(data.isValid == 1) {
//            printf("here\n");
            gmp_printf ("%Zd\n", data.value);
            printed++;
        } else {
            sleep(1);
        }
    }
    
    fflush(NULL);
    tSafeDestruct(numberList);
}
