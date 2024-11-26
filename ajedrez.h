#ifndef AJEDREZ_H
#define AJEDREZ_H

#include <gtk/gtk.h>
#include "kgchess.h"

// Declaracion de la variable global para el estado del juego
extern kgchess_t *juego;

// Declaraciones de las funciones
void mostrar_ganador(GtkWindow *parent, kgchess_player_t ganador);
void actualizar_estado(GtkGrid *grid, GtkWindow *parent);
void on_square_clicked(GtkButton *button, gpointer data);
GtkWidget* crear_tablero(GtkWindow *parent);
void presionar_el_boton_de_play(GtkButton *button, gpointer user_data);

#endif // AJEDREZ_H
