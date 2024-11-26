#include "ajedrez.h"

// Declaramos la variable global juego que representara el estado del juego de ajedrez.
kgchess_t *juego;

// Funcion para mostrar un mensaje con el ganador del juego.
void mostrar_ganador(GtkWindow *parent, kgchess_player_t ganador) {
    const char *mensaje;
    if (ganador == KGCHESS_PLAYER_WHITE) {
        mensaje = "El jugador Blanco gano!";
    } else if (ganador == KGCHESS_PLAYER_BLACK) {
        mensaje = "El jugador Negro gano!";
    } else {
        mensaje = "Es un empate!";
    }

    GtkWidget *dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Resultado del juego");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), parent);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_dialog_add_button(GTK_DIALOG(dialog), "_OK", GTK_RESPONSE_OK);


    GtkWidget *label = gtk_label_new(mensaje);
    gtk_box_append(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), label);
    // Crear una etiqueta para mostrar el mensaje y agregarla al dialogo


    gtk_widget_set_visible(dialog, TRUE);// Mostrar el dialogo
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
}

// Esta funcion lo que hace es mostrar el tablero actualizado tras mover una ficha y verificar si el juego termino.
void actualizar_estado(GtkGrid *grid, GtkWindow *parent) {
    // Recorremos cada celda del tablero de ajedrez que tiene un tamano de 8x8.
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) { 
            // Obtenemos el boton asociado a la posicion actual (col, row) en la cuadricula.
            GtkWidget *button = gtk_grid_get_child_at(grid, col, row);

            // Recuperamos la pieza de ajedrez que esta en la posicion actual del tablero (col, row).
            kgchess_piece_t piece = kgchess_get_piece_at(juego, col, row);

            // Declaramos un puntero gchar y le asignamos la pieza.
            gchar *label = NULL; // Si no hay pieza en la celda, este puntero permanecera como NULL.

            // Si existe una pieza en la celda (no es KGCHESS_PIECE_NONE), decidimos cual es el simbolo correcto.
            if (piece.type != KGCHESS_PIECE_NONE) {
                // Usamos un switch para determinar que tipo de pieza es (rey, reina, torre, etc.) y asignar el simbolo adecuado, dependiendo del jugador (blanco o negro).
                switch (piece.type) {
                    case KGCHESS_PIECE_KING:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♔" : "♚";
                        break;
                    case KGCHESS_PIECE_QUEEN:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♕" : "♛";
                        break;
                    case KGCHESS_PIECE_ROOK:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♖" : "♜";
                        break;
                    case KGCHESS_PIECE_BISHOP:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♗" : "♝";
                        break;
                    case KGCHESS_PIECE_KNIGHT:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♘" : "♞";
                        break;
                    case KGCHESS_PIECE_PAWN:
                        label = (piece.player == KGCHESS_PLAYER_WHITE) ? "♙" : "♟";
                        break;
                    default:
                        break; // Si por alguna razon no se reconoce el tipo, no hacemos nada.
                }
            }

            // Actualizamos la etiqueta del boton correspondiente.
            // Si label es NULL (no hay pieza), la celda se mostrara vacia.
            gtk_button_set_label(GTK_BUTTON(button), label ? label : "");
        }
    }

    // Verificamos si el juego ha terminado y mostramos el ganador.
    if (kgchess_get_state(juego) == KGCHESS_STATE_ENDED) {
        kgchess_player_t ganador = kgchess_get_winner(juego);
        mostrar_ganador(parent, ganador); // Llamamos a la funcion para mostrar el ganador.
    }
}

// Funcion para habilitar los clicks en las casillas del tablero.
// Esta funcion maneja la seleccion de origen y destino para mover una pieza.
// Verifica si es el turno del jugador, valida el movimiento y actualiza la interfaz grafica.
void on_square_clicked(GtkButton *button, gpointer data) {
    // Inicialmente, la posicion es {-1, -1}, lo que indica que no se ha seleccionado ninguna casilla.
    static kgchess_pos_t from = {-1, -1};

    // Convertimos el puntero data a un puntero kgchess_pos_t.
    // Esto nos permite acceder a las coordenadas (x, y) de la casilla que fue clicada.
    kgchess_pos_t *pos = (kgchess_pos_t *)data;

    // Obtenemos la pieza que se encuentra en la casilla clicada utilizando la funcion de la biblioteca kgchess.
    kgchess_piece_t pieza = kgchess_get_piece_at(juego, pos->x, pos->y);

    // Nos aseguramos de que la pieza seleccionada pertenezca al jugador que esta en turno.
    if (from.x == -1 && pieza.player != kgchess_get_current_player(juego)) {
        // Si no es el turno del jugador que intenta mover la pieza, mostramos un mensaje en la consola y no permitimos realizar ninguna accion.
        printf("No es tu turno.\n");
        return; // Finalizamos la funcion para evitar que se seleccione una pieza del oponente.
    }

    // Si no hay una posicion de origen seleccionada, asignamos la casilla clicada como origen.
    if (from.x == -1) {
        from = *pos; // Guardamos las coordenadas de la casilla clicada en "from".
    } else {
        // Si ya hay una posicion de origen seleccionada, definimos un movimiento desde "from" hacia "pos". El movimiento incluye informacion sobre origen, destino y flags adicionales (en este caso, no es un ataque especial, enroque ni captura al paso).
        kgchess_move_t move = {from, *pos, false, false, false};

        // Intentamos realizar el movimiento utilizando la funcion "kgchess_move".
        // Si el movimiento es valido:
        if (kgchess_move(juego, move)) {
            // Actualizamos el estado del tablero en la interfaz grafica.
            // Esto asegura que el tablero muestre las posiciones actualizadas de las piezas.
            GtkWindow *parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(button)));
            actualizar_estado(GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(button))), parent);
        } else {
            // Si el movimiento no es valido, mostramos un mensaje en la consola indicando el error.
            printf("Movimiento no valido.\n");
        }
        // Reiniciamos la posicion de origen para permitir seleccionar una nueva pieza en el proximo clic.
        from.x = -1;
    }
}

// Funcion que crea el tablero de ajedrez (8x8) como una cuadricula de botones.
GtkWidget* crear_tablero(GtkWindow *parent) {
    // Creamos una cuadricula (GtkGrid) que contendra los botones del tablero.
    GtkWidget *grid = gtk_grid_new();

    // Inicializamos el juego de ajedrez creando una nueva instancia del tablero.
    juego = kgchess_make();

    // Recorremos las 64 celdas del tablero.
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Creamos un boton para representar cada celda del tablero.
            GtkWidget *button = gtk_button_new();
            gtk_widget_set_size_request(button, 50, 50); // Establecemos el tamano del boton.

            // Le damos el color de la casilla
            const char *color_class = (((7 - row) + col) % 2 == 0) ? "white-square" : "black-square";
            gtk_widget_add_css_class(button, color_class);

            // Guardamos la posicion actual (fila y columna) asociada a este boton.
            kgchess_pos_t *pos = g_new(kgchess_pos_t, 1); // Reservamos memoria para kgchess_pos_t.
            pos->x = col;
            pos->y = row;

            // Conectamos el boton con la funcion on_square_clicked para manejar eventos de clic.
            g_signal_connect(button, "clicked", G_CALLBACK(on_square_clicked), pos);

            // Aniadimos el boton a la cuadricula en la posicion especificada.
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        }
    }

    // Actualizamos la cuadricula para mostrar las piezas en sus posiciones iniciales.
    actualizar_estado(GTK_GRID(grid), parent); // Corregido para incluir el argumento parent.

    // Retornamos la cuadricula del tablero para aniadirla a la ventana mas adelante.
    return grid;
}

// Funcion para el boton play que nos redirige al tablero
void presionar_el_boton_de_play(GtkButton *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data); // Obtenemos la ventana principal.

    GtkWidget *board = crear_tablero(window); // Pasamos el parent a crear_tablero.

    // Caja para centrar el tablero
    GtkWidget *center_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(center_box), board);

    gtk_window_set_child(window, center_box); // Reemplazamos el contenido de la ventana con el tablero centrado.
}

