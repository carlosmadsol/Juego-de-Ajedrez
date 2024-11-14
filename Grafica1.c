#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    // Inicializa GTK
    gtk_init();

    // Crea una nueva ventana
    GtkWidget *window = gtk_window_new();
    
    // Configura el título de la ventana
    gtk_window_set_title(GTK_WINDOW(window), "Mi Ventana en GTK 4");

    // Configura el tamaño de la ventana (ancho y alto en píxeles)
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Conecta la señal de "destroy" para cerrar la ventana
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Muestra la ventana
    gtk_widget_show(window);

    // Inicia el bucle principal de GTK
    gtk_main();

    return 0;
}
