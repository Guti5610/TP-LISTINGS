#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Obtain a binary semaphore's ID, allocating if necessary. */
int binary_semaphore_allocation(key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

/* Deallocate a binary semaphore. All users must have finished their
   use. Returns -1 on failure. */
int binary_semaphore_deallocate(int semid) {
    union semun ignored_argument;
    return semctl(semid, 1, IPC_RMID, ignored_argument);
}

int main() {
    key_t key = 1234;  /* arbitrary semaphore key */
    int semid;

    /* Allocate semaphore */
    semid = binary_semaphore_allocation(key, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore created with ID = %d\n", semid);

    /* Deallocate semaphore */
    if (binary_semaphore_deallocate(semid) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore with ID = %d deleted.\n", semid);

    return 0;
}

