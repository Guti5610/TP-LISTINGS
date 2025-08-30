#include <pthread.h>
#include <iostream>
using namespace std;

/* Clase que simula una excepción para indicar que un hilo debe terminar. */
class ThreadExitException {
public:
    /* Crea una excepción que señala la salida del hilo con RETURN_VALUE. */
    ThreadExitException(void* return_value)
        : thread_return_value_(return_value) {}

    /* Hace que el hilo termine, devolviendo el valor indicado en el constructor. */
    void* DoThreadExit() {
        pthread_exit(thread_return_value_);
        return nullptr; // nunca se ejecuta, pero evita warnings
    }

private:
    void* thread_return_value_;
};

/* Función de ejemplo que hace trabajo en bucle. */
void do_some_work() {
    while (true) {
        cout << "Trabajando en el hilo..." << endl;

        // Simulación: si se cumple la condición, pedimos terminar el hilo
        if (rand() % 10 == 0) {
            throw ThreadExitException((void*)"Fin del hilo por condición");
        }
    }
}

/* Función del hilo */
void* thread_function(void*) {
    try {
        do_some_work();
    }
    catch (ThreadExitException& ex) {
        /* Alguna condición indicó que debemos salir del hilo */
        ex.DoThreadExit();
    }
    return nullptr;
}

/* Programa principal */
int main() {
    pthread_t thread;

    // Crear un hilo que ejecuta thread_function
    pthread_create(&thread, nullptr, thread_function, nullptr);

    // Esperar a que el hilo termine
    void* thread_result;
    pthread_join(thread, &thread_result);

    cout << "El hilo terminó con mensaje: " << (char*)thread_result << endl;
    return 0;
}
