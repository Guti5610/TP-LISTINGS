#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Definición de un trabajo
struct job {
    struct job* next;  // Puntero al siguiente trabajo
    int id;            // Identificador del trabajo
};

// Cola global de trabajos pendientes
struct job* job_queue = NULL;

// Mutex que protege la cola
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

// Simulación de procesar un trabajo
void process_job(struct job* j) {
    printf("Procesando trabajo con id=%d\n", j->id);
}

// Función del hilo trabajador
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;

        // Bloquear el mutex antes de tocar la cola
        pthread_mutex_lock(&job_queue_mutex);

        if (job_queue == NULL)
            next_job = NULL;
        else {
            // Sacar el primer trabajo
            next_job = job_queue;
            job_queue = job_queue->next;
        }

        // Ya no necesitamos el mutex
        pthread_mutex_unlock(&job_queue_mutex);

        // Si no había trabajos -> salir
        if (next_job == NULL)
            break;

        // Procesar el trabajo
        process_job(next_job);

        // Liberar memoria
        free(next_job);
    }
    return NULL;
}

int main() {
    // Crear algunos trabajos en la cola
    for (int i = 0; i < 5; i++) {
        struct job* new_job = malloc(sizeof(struct job));
        new_job->id = i + 1;

        pthread_mutex_lock(&job_queue_mutex);
        new_job->next = job_queue;
        job_queue = new_job;
        pthread_mutex_unlock(&job_queue_mutex);
    }

    // Crear un hilo que procese los trabajos
    pthread_t worker;
    pthread_create(&worker, NULL, thread_function, NULL);

    // Esperar a que termine
    pthread_join(worker, NULL);

    printf("Todos los trabajos fueron procesados.\n");
    return 0;
}
