// Incluimos las bibliotecas necesarias:
// - `gtk/gtk.h` La usamos para crear interfaces gráficas con GTK.
// - `kgchess.h` Lleva la lógica del juego de ajedrez.
#include <gtk/gtk.h>
#include "kgchess.h"

// Declaramos una variable global juego que representará el estado del juego de ajedrez.
// Esta estructura será inicializada cuando el tablero sea creado y utilizada para registrar movimientos.
kgchess_t *juego;

// Esta funcion lo que hace es mostrar el tablero actualizado tras mover una ficha
void actualizar_estado(GtkGrid *grid) {
    // Recorremos cada celda del tablero de ajedrez que tiene un tamaño de 8x8.
    for (int row = 0; row < 8; row++) { // Iteramos sobre cada fila y columna ya que es basado en el manejo de matrices visto en el curso
        for (int col = 0; col < 8; col++) { 
            // Obtenemos el botón asociado a la posición actual (col, row) en la cuadrícula.
            GtkWidget *button = gtk_grid_get_child_at(grid, col, row);

            // Recuperamos la pieza de ajedrez que está en la posición actual del tablero (col, row).
            kgchess_piece_t piece = kgchess_get_piece_at(juego, col, row);

            // Declaramos un puntero gchar y le asignamos la pieza.
            gchar *label = NULL; // Si no hay pieza en la celda, este puntero permanecerá como NULL.

            // Si existe una pieza en la celda (no es KGCHESS_PIECE_NONE), decidimos cuál es el símbolo correcto.
            if (piece.type != KGCHESS_PIECE_NONE) {
                // Usamos un switch para determinar qué tipo de pieza es (rey, reina, torre, etc.) y asignar el símbolo adecuado, dependiendo del jugador (blanco o negro).
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
                        break; // Si por alguna razón no se reconoce el tipo, no hacemos nada.
                }
            }

            // Actualizamos la etiqueta del botón correspondiente.
            // Si label es NULL (no hay pieza), la celda se mostrará vacía.
            gtk_button_set_label(GTK_BUTTON(button), label ? label : "");
        }
    }
}

//Funcion para habilitar los clicks
void on_square_clicked(GtkButton *button, gpointer data) {
    // Guardamos la posición de origen del movimiento de una pieza.
    // Lo que hacemos es asignar desde el inicio una posicion de x=-1 y y=-1, de esta forma no estamos posicionados sobre ninguna pieza del tablero al iniciar el turno, dandonos libertad
    static kgchess_pos_t from = {-1, -1};

    // Obtenemos la posición de la celda actual (donde se hizo clic).
    kgchess_pos_t *pos = (kgchess_pos_t *)data;

    if (from.x == -1) {
        // Si no hay celda de origen seleccionada, marcamos esta celda como el origen del movimiento.
        from = *pos;
    } else {
        // Si ya tenemos un origen seleccionado, intentamos mover la pieza a la celda actual (destino).
        kgchess_move_t move = {from, *pos, false, false, false};

        // La función `kgchess_move` verifica si el movimiento es válido.
        if (kgchess_move(juego, move)) {
            // Si el movimiento es válido, actualizamos el tablero para reflejar los nuevos cambios.
            actualizar_estado(GTK_GRID(gtk_widget_get_parent(GTK_WIDGET(button))));
        }

        // Después de realizar el movimiento (o intentar), reseteamos el origen para el próximo turno.
        from.x = -1;
    }
}

// Función que crea el tablero de ajedrez (8x8) como una cuadrícula de botones.
GtkWidget* crear_tablero() {
    // Creamos una cuadrícula (`GtkGrid`) que contendrá los botones del tablero.
    GtkWidget *grid = gtk_grid_new();

    // Inicializamos el juego de ajedrez creando una nueva instancia del tablero.
    juego = kgchess_make();

    // Recorremos las 64 celdas del tablero.
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Creamos un botón para representar cada celda del tablero.
            GtkWidget *button = gtk_button_new();
            gtk_widget_set_size_request(button, 50, 50); // Establecemos el tamaño del botón.

            // Guardamos la posición actual (fila y columna) asociada a este botón.
            kgchess_pos_t *pos = g_new(kgchess_pos_t, 1); // Reservamos memoria para `kgchess_pos_t`.
            pos->x = col;
            pos->y = row;

            // Conectamos el botón con la función `on_square_clicked` para manejar eventos de clic.
            g_signal_connect(button, "clicked", G_CALLBACK(on_square_clicked), pos);

            // Añadimos el botón a la cuadrícula en la posición especificada.
            gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        }
    }

    // Actualizamos la cuadrícula para mostrar las piezas en sus posiciones iniciales.
    actualizar_estado(GTK_GRID(grid));

    // Retornamos la cuadrícula del tablero para añadirla a la ventana más adelante.
    return grid;
}

// Función para el boton play que nos redirige al tablero
void presionar_el_boton_de_play(GtkButton *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data); // Obtenemos la ventana principal.

    GtkWidget *board = crear_tablero(); // Creamos el tablero de ajedrez.
    gtk_window_set_child(window, board); // Reemplazamos el contenido de la ventana con el tablero.
}

// Función para activar la aplicación.
// Crea la ventana inicial y muestra el botón "Play".
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app); // Creamos una nueva ventana.
    gtk_window_set_title(GTK_WINDOW(window), "Ajedrez con kgchess"); // Establecemos el título de la ventana.
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600); // Definimos el tamaño inicial de la ventana.

    // Creamos un botón que dice "Play", el cual se usará para iniciar el juego.
    GtkWidget *play_button = gtk_button_new_with_label("Play");
    gtk_window_set_child(GTK_WINDOW(window), play_button); // Añadimos el botón a la ventana.

    // Conectamos el evento "clicked" del botón con la función `presionar_el_boton_de_play`.
    g_signal_connect(play_button, "clicked", G_CALLBACK(presionar_el_boton_de_play), window);

    gtk_widget_set_visible(window, TRUE); // Hacemos visible la ventana.
}

// Función principal del programa. Es el punto de entrada.
int main(int argc, char *argv[]) {
    // Creamos una nueva aplicación GTK.
    GtkApplication *app = gtk_application_new("org.example.chess", G_APPLICATION_DEFAULT_FLAGS);

    // Conectamos la señal "activate" de la aplicación con la función `activate`.
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Ejecutamos la aplicación y manejamos su ciclo de vida (bucle principal de eventos).
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Liberamos la memoria asociada a la aplicación una vez que termina.
    g_object_unref(app);

    return status; // Retornamos el estado final de la ejecución del programa.
}
