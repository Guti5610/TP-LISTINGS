#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

/* Allocate a temporary buffer. */
void* allocate_buffer(size_t size) {
    return malloc(size);
}

/* Deallocate a temporary buffer. */
void deallocate_buffer(void* buffer) {
    free(buffer);
}

void* do_some_work(void* arg) {
    /* Allocate a temporary buffer. */
    void* temp_buffer = allocate_buffer(1024);

    if (!temp_buffer) {
        fprintf(stderr, "Error allocating buffer\n");
        pthread_exit(NULL);
    }

    /* Register a cleanup handler for this buffer, 
       to deallocate it in case the thread exits or is cancelled. */
    pthread_cleanup_push(deallocate_buffer, temp_buffer);

    printf("Thread is working... allocated buffer at %p\n", temp_buffer);

    /* Simulate some work that could be cancelled. */
    for (int i = 0; i < 5; i++) {
        printf("Work step %d...\n", i + 1);
        sleep(1);
    }

    /* Unregister the cleanup handler.
       Passing 1 means execute it now (free buffer). */
    pthread_cleanup_pop(1);

    return NULL;
}

int main() {
    pthread_t thread;

    /* Create a thread that does some work. */
    if (pthread_create(&thread, NULL, do_some_work, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    /* Optionally, we could cancel the thread here to test cleanup:
       pthread_cancel(thread);
    */

    /* Wait for the thread to finish. */
    pthread_join(thread, NULL);

    printf("Main thread finished.\n");
    return 0;
}

