#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Initialize a binary semaphore with a value of 1. */
int binary_semaphore_initialize(int semid) {
    union semun argument;
    unsigned short values[1];
    values[0] = 1;
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

int main() {
    key_t key = 1234;   /* arbitrary semaphore key */
    int semid;

    /* Create a semaphore set containing 1 semaphore. */
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore created with ID = %d\n", semid);

    /* Initialize the semaphore with value 1. */
    if (binary_semaphore_initialize(semid) == -1) {
        perror("semctl - SETALL");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore initialized to 1.\n");

    /* Remove the semaphore after test. */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl - IPC_RMID");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore with ID = %d deleted.\n", semid);

    return 0;
}
