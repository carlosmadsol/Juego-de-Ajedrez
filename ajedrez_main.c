#include "ajedrez.h"
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

    // Crear una caja vertical para el título y el botón Play
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    // Título de bienvenida
    GtkWidget *title = gtk_label_new("Bienvenido a nuestro programa de ajedrez");
    gtk_widget_add_css_class(title, "title-label");
    gtk_box_append(GTK_BOX(vbox), title);

    // Botón Play estilizado
    GtkWidget *play_button = gtk_button_new_with_label("Play");
    gtk_widget_add_css_class(play_button, "play-button");
    gtk_box_append(GTK_BOX(vbox), play_button);

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
