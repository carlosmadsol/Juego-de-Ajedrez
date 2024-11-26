# Nombre del ejecutable
TARGET = ajedrez

# Compilador
CC = gcc

# Flags del compilador
CFLAGS = -Wall -g `pkg-config --cflags gtk4`

# Librerías necesarias
LIBS = `pkg-config --libs gtk4`

# Archivos fuente
SRC = ajedrez_main.c kgchess/kgchess.c

# Archivos objeto generados automáticamente
OBJ = $(SRC:.c=.o)

# Carpeta de inclusión para encabezados (opcional si tienes varios headers)
INCLUDES = -Ikgchess

# Regla principal
all: $(TARGET)

# Cómo compilar el programa
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

# Cómo generar los archivos objeto
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJ) $(TARGET)
