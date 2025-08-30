#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para representar un trabajo
struct job {
    struct job* next;  // Puntero al siguiente trabajo
    int id;            // ID del trabajo (ejemplo)
};

// Cola global de trabajos pendientes
struct job* job_queue = NULL;

// Procesar un trabajo (dummy, solo imprime)
void process_job(struct job* j) {
    printf("Procesando trabajo con id=%d\n", j->id);
}

// Función que será ejecutada por un hilo
void* thread_function(void* arg) {
    while (job_queue != NULL) {
        // Obtener el siguiente trabajo
        struct job* next_job = job_queue;

        // Avanzar la cola
        job_queue = job_queue->next;

        // Procesar el trabajo
        process_job(next_job);

        // Liberar memoria
        free(next_job);
    }
    return NULL;
}

int main() {
    // Crear una lista de trabajos
    for (int i = 0; i < 5; i++) {
        struct job* new_job = (struct job*) malloc(sizeof(struct job));
        new_job->id = i + 1;
        new_job->next = job_queue;
        job_queue = new_job;
    }

    // Crear un hilo que procese los trabajos
    pthread_t worker;
    pthread_create(&worker, NULL, thread_function, NULL);

    // Esperar a que termine
    pthread_join(worker, NULL);

    printf("Todos los trabajos fueron procesados.\n");
    return 0;
}
