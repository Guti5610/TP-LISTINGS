# Compilador y flags
CC = gcc
CFLAGS = -Wall -g

# Archivos fuente individuales
SRC = $(wildcard *.c)
EXEC = $(SRC:.c=)

# Proyecto grande (si existe)
# 👇 aquí defines cuáles .c forman parte del proyecto principal
PROJECT = miapp
PROJECT_SRC = main.c utils.c db.c
PROJECT_OBJ = $(PROJECT_SRC:.c=.o)

# ---- Reglas ----

# Por defecto: compila todo (independientes + proyecto grande)
all: $(EXEC) $(PROJECT)

# Compilar ejecutables individuales (un .c -> un binario)
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Compilar proyecto grande
$(PROJECT): $(PROJECT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(PROJECT_OBJ)

# Regla genérica para objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Ejecutar un binario independiente o el proyecto
run:
	@if [ -n "$(FILE)" ]; then \
		echo "Ejecutando $(FILE)..."; \
		./$(FILE); \
	else \
		echo "Usa: make run FILE=ejecutable"; \
	fi

# Limpiar
clean:
	rm -f $(EXEC) $(PROJECT) *.o

