#include <stdio.h>
#include <stdlib.h>
#include "prog2_1.c"
#include <gmp.h>
#include <stdarg.h>


mpz_t globalCounter;
pthread_mutex_t counterguard = PTHREAD_MUTEX_INITIALIZER;
TSAFELIST *numberList;

int main(int argc, char** argv) {
  numberList = tSafeConstruct();
  mpz_init_set_ui(globalCounter, 1);
}

void getNextNum(mpz_t * num) {
  pthread_mutex_lock(&counterguard);
  mpz_add_ui(globalCounter, globalCounter, 2);
  mpz_set(*num, globalCounter);
  pthread_mutex_unlock(&counterguard);
}

void * generatePrimes(void * args) {
  int prob = 0;
  int i = 0;
  mpz_t tempCounter;
  mpz_init_set(tempCounter, globalCounter);
  while(i < 100000) {
    getNextNum(&tempCounter);
    prob = mpz_probab_prime_p(tempCounter, 6);
  }
}
