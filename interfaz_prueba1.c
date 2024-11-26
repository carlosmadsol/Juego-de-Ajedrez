// Incluimos las bibliotecas necesarias:
// - gtk/gtk.h La usamos para crear interfaces graficas con GTK.
// - kgchess.h Lleva la logica del juego de ajedrez.
#include <gtk/gtk.h>
#include "kgchess.h"

// Declaramos una variable global juego que representara el estado del juego de ajedrez.
// Esta estructura sera inicializada cuando el tablero sea creado y utilizada para registrar movimientos.
kgchess_t *juego;

// Esta funcion lo que hace es mostrar el tablero actualizado tras mover una ficha
void actualizar_estado(GtkGrid *grid) {
    // Recorremos cada celda del tablero de ajedrez que tiene un tamano de 8x8.
    for (int row = 0; row < 8; row++) { // Iteramos sobre cada fila y columna ya que es basado en el manejo de matrices visto en el curso
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
}

// Funcion para habilitar los clicks
void on_square_clicked(GtkButton *button, gpointer data) {
    static kgchess_pos_t from = {-1, -1};
    kgchess_pos_t *pos = (kgchess_pos_t *)data;

    // Verifica si la pieza pertenece al jugador actual
    kgchess_piece_t pieza = kgchess_get_piece_at(juego, pos->x, pos->y);
    if (from.x == -1 && pieza.player != kgchess_get_current_player(juego)) {
        printf("No es tu turno.\n");
        return; // No permite mover una pieza del oponente.
    }

    if (from.x == -1) {
        from = *pos;
    } else {
        kgchess_move_t move = {from, *pos, false, false, false};

        if (kgchess_move(juego, move)) {
            actualizar_estado(GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(button))));
        } else {
            printf("Movimiento no válido.\n");
        }
        from.x = -1;
    }
}

// Funcion que crea el tablero de ajedrez (8x8) como una cuadricula de botones.
GtkWidget* crear_tablero() {
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
    actualizar_estado(GTK_GRID(grid));

    // Retornamos la cuadricula del tablero para aniadirla a la ventana mas adelante.
    return grid;
}

// Funcion para el boton play que nos redirige al tablero
void presionar_el_boton_de_play(GtkButton *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data); // Obtenemos la ventana principal.

    GtkWidget *board = crear_tablero(); // Creamos el tablero de ajedrez.
    gtk_window_set_child(window, board); // Reemplazamos el contenido de la ventana con el tablero.
}

// Funcion para activar la aplicacion.
// Crea la ventana inicial y muestra el boton Play.
// Le da color a las casillas
static void activate(GtkApplication *app, gpointer user_data) {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "color.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Ajedrez con kgchess");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

    GtkWidget *play_button = gtk_button_new_with_label("Play");
    gtk_window_set_child(GTK_WINDOW(window), play_button);

    g_signal_connect(play_button, "clicked", G_CALLBACK(presionar_el_boton_de_play), window);
    gtk_widget_set_visible(window, TRUE);
}

// Funcion principal del programa. Es el punto de entrada.
int main(int argc, char *argv[]) {
    // Creamos una nueva aplicacion GTK.
    GtkApplication *app = gtk_application_new("org.example.chess", G_APPLICATION_DEFAULT_FLAGS);

    // Conectamos la senal activate de la aplicacion con la funcion activate.
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Ejecutamos la aplicacion y manejamos su ciclo de vida (bucle principal de eventos).
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Liberamos la memoria asociada a la aplicacion una vez que termina.
    g_object_unref(app);

    return status; // Retornamos el estado final de la ejecucion del programa.
}
