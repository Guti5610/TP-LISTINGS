#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  // for sleep

int thread_flag;
pthread_mutex_t thread_flag_mutex;

void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    thread_flag = 0;
}

/* Dummy work function */
void do_work() {
    printf("Thread is doing work!\n");
    sleep(1);  // simulate work
}

/* Calls do_work repeatedly while the thread flag is set; otherwise spins. */
void* thread_function(void* thread_arg) {
    while (1) {
        int flag_is_set;

        /* Protect the flag with a mutex lock. */
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (flag_is_set)
            do_work();
        /* Else don't do anything. Just loop again. */
    }
    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */
void set_thread_flag(int flag_value) {
    /* Protect the flag with a mutex lock. */
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_mutex_unlock(&thread_flag_mutex);
}

int main() {
    pthread_t thread;

    initialize_flag();

    /* Create the worker thread */
    pthread_create(&thread, NULL, thread_function, NULL);

    /* Turn the flag on and off a few times */
    for (int i = 0; i < 5; i++) {
        printf("Main: turning thread_flag ON\n");
        set_thread_flag(1);
        sleep(3);  // let thread do work

        printf("Main: turning thread_flag OFF\n");
        set_thread_flag(0);
        sleep(2);  // thread spins but does nothing
    }

    /* Normally you would signal the thread to exit cleanly.
       For simplicity, we just exit here. */
    printf("Main thread exiting.\n");
    return 0;
}
