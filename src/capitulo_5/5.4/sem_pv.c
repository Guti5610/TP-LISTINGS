#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

/* Wait on a binary semaphore. Block until the semaphore value is positive,
   then decrement it by 1. */
int binary_semaphore_wait(int semid) {
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Decrement by 1. */
    operations[0].sem_op = -1;
    /* Permit undo'ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

/* Post to a binary semaphore: increment its value by 1.
   This returns immediately. */
int binary_semaphore_post(int semid) {
    struct sembuf operations[1];
    /* Use the first (and only) semaphore. */
    operations[0].sem_num = 0;
    /* Increment by 1. */
    operations[0].sem_op = 1;
    /* Permit undo'ing. */
    operations[0].sem_flg = SEM_UNDO;
    return semop(semid, operations, 1);
}

/* Small helper to initialize semaphore to 1 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int binary_semaphore_initialize(int semid) {
    union semun argument;
    unsigned short values[1];
    values[0] = 1;
    argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

int main() {
    key_t key = 1234;  /* arbitrary semaphore key */
    int semid;

    /* Create a semaphore set with one semaphore. */
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore created with ID = %d\n", semid);

    /* Initialize it to 1. */
    if (binary_semaphore_initialize(semid) == -1) {
        perror("semctl - SETALL");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore initialized to 1\n");

    printf("Main: waiting on semaphore...\n");
    if (binary_semaphore_wait(semid) == -1) {
        perror("semop - wait");
        exit(EXIT_FAILURE);
    }
    printf("Main: semaphore acquired! Critical section.\n");

    sleep(3);  /* simulate critical section work */

    printf("Main: posting to semaphore...\n");
    if (binary_semaphore_post(semid) == -1) {
        perror("semop - post");
        exit(EXIT_FAILURE);
    }
    printf("Main: semaphore released.\n");

    /* Clean up */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl - IPC_RMID");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore deleted.\n");

    return 0;
}
