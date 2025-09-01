# Compilador y banderas
CC = gcc
CXX = g++
CFLAGS = -Wall -g -std=c11 -D_POSIX_C_SOURCE=200809L
CXXFLAGS = -Wall -g -std=c++11 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -ltiff -lpthread

# Directorios
SRC_DIR = src
BIN_DIR = bin

# Encontrar todos los directorios de listings que tienen archivos fuente
LISTING_DIRS = $(shell find $(SRC_DIR) -type d -exec sh -c ' \
    if ls "$$1"/*.c >/dev/null 2>&1 || ls "$$1"/*.cpp >/dev/null 2>&1; then \
        echo "$$1"; \
    fi' sh {} \;)

# Default target - solo compila directorios con archivos fuente
.PHONY: all
all: $(foreach dir,$(LISTING_DIRS),$(dir:$(SRC_DIR)/%=$(BIN_DIR)/%/programa))

# Regla para compilar listings con múltiples archivos (C y C++ mezclados)
$(BIN_DIR)/%/programa: $(SRC_DIR)/%/*
	@echo "Compilando listing: $*"
	@mkdir -p $(dir $@)
	@cd $(SRC_DIR)/$* && \
	if ls *.c >/dev/null 2>&1 && ls *.cpp >/dev/null 2>&1; then \
		# Compilar C++ y luego enlazar con C \
		$(CXX) $(CXXFLAGS) -c *.cpp -o ../../../$(dir $@)/cpp_files.o; \
		$(CC) $(CFLAGS) *.c ../../../$(dir $@)/cpp_files.o -o ../../../$@ $(LDFLAGS); \
		rm -f ../../../$(dir $@)/cpp_files.o; \
		echo "Compilado mixto C/C++: $@"; \
	elif ls *.cpp >/dev/null 2>&1; then \
		# Solo archivos C++ \
		$(CXX) $(CXXFLAGS) *.cpp -o ../../../$@ $(LDFLAGS); \
		echo "Compilado C++: $@"; \
	elif ls *.c >/dev/null 2>&1; then \
		# Solo archivos C \
		$(CC) $(CFLAGS) *.c -o ../../../$@ $(LDFLAGS); \
		echo "Compilado C: $@"; \
	else \
		echo "No hay archivos fuente en: $(SRC_DIR)/$*"; \
		exit 1; \
	fi
# Compilar listing específico
.PHONY: listing-%
listing-%:
	@listing_name=$(subst listing-,,$@); \
	chapter_num=$$(echo $$listing_name | cut -d. -f1); \
	listing_dir="src/capitulo-$$chapter_num/$$listing_name"; \
	if [ ! -d "$$listing_dir" ]; then \
		echo "No se encontró el directorio: $$listing_dir"; \
		exit 1; \
	fi; \
	if ! ls "$$listing_dir"/*.c >/dev/null 2>&1 && ! ls "$$listing_dir"/*.cpp >/dev/null 2>&1; then \
		echo "No hay archivos fuente en: $$listing_dir"; \
		exit 1; \
	fi; \
	bin_dir="bin/capitulo-$$chapter_num/$$listing_name"; \
	mkdir -p "$$bin_dir"; \
	cd "$$listing_dir" && \
	if ls *.c >/dev/null 2>&1 && ls *.cpp >/dev/null 2>&1; then \
		$(CXX) $(CXXFLAGS) -c *.cpp -o "../../../$$bin_dir/cpp_files.o"; \
		$(CC) $(CFLAGS) *.c "../../../$$bin_dir/cpp_files.o" -o "../../../$$bin_dir/programa" $(LDFLAGS); \
		rm -f "../../../$$bin_dir/cpp_files.o"; \
		echo "Compilado mixto listing $$listing_name: $$bin_dir/programa"; \
	elif ls *.cpp >/dev/null 2>&1; then \
		$(CXX) $(CXXFLAGS) *.cpp -o "../../../$$bin_dir/programa" $(LDFLAGS); \
		echo "Compilado C++ listing $$listing_name: $$bin_dir/programa"; \
	elif ls *.c >/dev/null 2>&1; then \
		$(CC) $(CFLAGS) *.c -o "../../../$$bin_dir/programa" $(LDFLAGS); \
		echo "Compilado C listing $$listing_name: $$bin_dir/programa"; \
	fi


# Limpiar binarios
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*
	@echo "Todos los binarios eliminados"

# Mostrar listings disponibles (solo los que tienen archivos)
.PHONY: list
list:
	@echo "Listings disponibles con archivos fuente:"
	@for dir in $(LISTING_DIRS); do \
		listing=$${dir#$(SRC_DIR)/}; \
		echo "  - $$listing"; \
	done

# Ayuda
.PHONY: help
help:
	@echo "Makefile para TP-LISTINGS"
	@echo ""
	@echo "Uso:"
	@echo "  make all              Compilar todos los listings con archivos"
	@echo "  make clean            Eliminar todos los binarios"
	@echo "  make listing-X.Y      Compilar listing específico (ej: make listing-1.1)"
	@echo "  make list             Mostrar listings disponibles"
	@echo "  make help             Mostrar esta ayuda"