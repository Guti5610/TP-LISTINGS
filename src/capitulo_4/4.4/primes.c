#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>  // Se añade para malloc y free

/* Compute successive prime numbers (very inefficiently). Return the
   Nth prime number, where N is the value pointed to by *ARG. */
void* compute_prime(void* arg) {
    int candidate = 2;
    int n = *((int*) arg);
    
    while (1) {
        int factor;
        int is_prime = 1;
        
        /* Test primality by successive division. */
        for (factor = 2; factor < candidate; ++factor)
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }
        
        /* Is this the prime number we're looking for? */
        if (is_prime) {
            if (--n == 0) {
                /* Return the desired prime number as the thread return value. */
                // CORRECCIÓN: Se usa malloc para evitar casteo directo int -> pointer
                int* result = (int*) malloc(sizeof(int));
                *result = candidate;
                return (void*) result;  // Ahora retorna un puntero válido
            }
        }
        ++candidate;
    }
    return NULL;
}

int main() {
    pthread_t thread;
    int which_prime = 5000;
    int* prime;  // CORRECCIÓN: Ahora es un puntero para recibir el resultado
    
    /* Start the computing thread, up to the 5,000th prime number. */
    pthread_create(&thread, NULL, &compute_prime, &which_prime);
    
    /* Do some other work here... */
    
    /* Wait for the prime number thread to complete, and get the result. */
    // CORRECCIÓN: Se recibe como void** para puntero a entero
    pthread_join(thread, (void**) &prime);
    
    /* Print the largest prime it computed. */
    printf("The %dth prime number is %d.\n", which_prime, *prime);
    
    // CORRECCIÓN: Se libera la memoria asignada en compute_prime
    free(prime);
    
    return 0;
}