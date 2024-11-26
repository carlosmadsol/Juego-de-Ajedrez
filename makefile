# Variable para el compilador que usaremos (en este caso, gcc).
CC = gcc

# Flags para la compilación:
# - Wall: Activa todos los mensajes de advertencia del compilador.
# - g: Incluye información de depuración para herramientas como gdb.
# - pkg-config: Se usa para obtener las opciones de compilación de GTK4.
CFLAGS = -Wall -g `pkg-config --cflags gtk4` -Ikgchess


# Flags para la vinculación (linking), que incluyen las bibliotecas necesarias para GTK4.
LDFLAGS = `pkg-config --libs gtk4`

# Lista de los archivos objeto (.o) que se generarán y enlazarán para crear el ejecutable.
OBJ = ajedrez.o ajedrez_main.o kgchess/kgchess.o

# Regla por defecto que se ejecuta cuando se invoca `make`. 
# En este caso, se compila el ejecutable principal llamado `ajedrez`.
all: ajedrez

# Regla para construir el ejecutable final `ajedrez`.
# Toma todos los archivos objeto especificados en $(OBJ) y los enlaza usando las bibliotecas especificadas en $(LDFLAGS).
ajedrez: $(OBJ)
	$(CC) -o ajedrez $(OBJ) $(LDFLAGS)

# Regla para compilar ajedrez.o:
# - Toma como entrada `ajedrez.c` y su archivo de encabezado correspondiente `ajedrez.h`.
# - Usa las banderas de compilación definidas en $(CFLAGS) para generar `ajedrez.o`.
ajedrez.o: ajedrez.c ajedrez.h
	$(CC) $(CFLAGS) -c ajedrez.c -o ajedrez.o

# Regla para compilar ajedrez_main.o:
# - Toma como entrada `ajedrez_main.c` y su encabezado `ajedrez.h`.
# - Genera el archivo objeto `ajedrez_main.o`.
ajedrez_main.o: ajedrez_main.c ajedrez.h
	$(CC) $(CFLAGS) -c ajedrez_main.c -o ajedrez_main.o

# Regla para compilar el módulo de kgchess:
# - Toma como entrada `kgchess.c` y su encabezado `kgchess.h`, ambos en el subdirectorio kgchess.
# - Genera el archivo objeto `kgchess.o` en el mismo subdirectorio.
kgchess/kgchess.o: kgchess/kgchess.c kgchess/kgchess.h
	$(CC) $(CFLAGS) -c kgchess/kgchess.c -o kgchess/kgchess.o

# Regla para limpiar los archivos generados durante la compilación:
# - Elimina todos los archivos objeto (.o) listados en $(OBJ).
# - Elimina también el ejecutable `ajedrez`.
clean:
	rm -f $(OBJ) ajedrez
