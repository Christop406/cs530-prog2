# CS530 Program 2

## Program Description
This program is a prime number generator designed to test our knowledge of the C programming language, multi-threaded and reentrant code, and our data structures knowledge.

## File Description

`prog2_1.c`
A thread-safe linked list implementation. Acts as a FIFO queue. It is used in the later parts of the program to store the found numbers.

`prog2_1.h`
The header file describing the methods exposed by `prog2_1.c`. It is included in `prog2_1.c`.

`prog2_2.c`
The implementation of the prime number generator. The program takes in 2 arguments - **K** (number of primes) and **B** (Bit-length). The program is run with the command `./prog2_2 <K> <B>`. At runtime, the program gets the first odd number of bit-length **B**, and continually increments a global counter variable by two (since we don't *ever* need to check even numbers). Upon each iteration, the function `mpz_probab_prime_p()` from the `GMP` library runs a Miller-Rabin primality check with 100,000 iterations. This is overkill. It slows the program down so that it is easy to check if the code is properly multi-threaded. After each prime number is found, it will print within the next second. If no primes are found within the second, nothing prints. It will continue this loop until it finds **K** prime numbers.


`// By Chris Gilardi`
