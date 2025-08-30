#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
char* read_from_file (const char* filename, size_t length)
{
char* buffer;
int fd;
ssize_t bytes_read;
/* Allocate the buffer. */
buffer = (char*) malloc (length);
if (buffer == NULL)
return NULL;
/* Open the file. */
fd = open (filename, O_RDONLY);
if (fd == -1) {
/* open failed. Deallocate buffer before returning. */
free (buffer);
return NULL;
}
/* Read the data. */
bytes_read = read (fd, buffer, length);
if (bytes_read != length) {
/* read failed. Deallocate buffer and close fd before returning.*/
free (buffer);
close (fd);
return NULL;
}
/* Everything's fine. Close the file and return the buffer. */
close (fd);
return buffer;
}
int main() {
    const char* filename = "archivo.txt";
    const char* mensaje = "Hola, este es un archivo de prueba.\n";

    /* Creamos un archivo de prueba */
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return 1;
    }
    fputs(mensaje, f);
    fclose(f);

    /* Leemos el archivo usando la función */
    size_t length = strlen(mensaje); // cantidad de bytes a leer
    char* contenido = read_from_file(filename, length);

    if (contenido == NULL) {
        printf("Error al leer el archivo.\n");
        return 1;
    }

    /* Mostramos el contenido */
    printf("Contenido leído:\n%.*s", (int)length, contenido);

    /* Liberamos memoria */
    free(contenido);

    return 0;
}
