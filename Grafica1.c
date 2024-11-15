#include <gtk/gtk.h>

// Función que se ejecuta cuando la aplicación se activa
static void on_activate(GtkApplication *app, gpointer user_data) {
    // Crear una nueva ventana vinculada a la aplicación
    GtkWidget *window = gtk_application_window_new(app);

    // Configurar la ventana
    gtk_window_set_title(GTK_WINDOW(window), "Ventana GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Hacer visible la ventana
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    // Crear una nueva instancia de GtkApplication
    GtkApplication *app = gtk_application_new("com.example.gtkapp", G_APPLICATION_FLAGS_NONE);

    // Conectar la señal "activate" al manejador de activación
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Ejecutar la aplicación (reemplaza gtk_main())
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Liberar la memoria asignada a la aplicación
    g_object_unref(app);

    return status;
}
