#include <gtk/gtk.h>
#include <glib/gstdio.h>

static void
print_hello(GtkWidget *widget,
            gpointer data)
{
  g_print("Hello World\n");
}

static void
quit_cb(GtkWindow *window)
{
  gtk_window_close(window);
}

static void
inspect_renderer(GObject *window)
{
  // Get the surface and the renderer used by the window
  GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window));
  GskRenderer *renderer = gtk_native_get_renderer(GTK_NATIVE(window));
  g_print("Current Backend (Windowing Protocol): %s\n", G_OBJECT_TYPE_NAME(gdk_display_get_default()));
  g_print("Graphics Library (Rendering API) in use: %s\n", G_OBJECT_TYPE_NAME(renderer));
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
  /* Construct a GtkBuilder instance and load our UI description */
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "examples/builder2.ui", NULL);

  /* Connect signal handlers to the constructed widgets. */
  GObject *window = gtk_builder_get_object(builder, "window");
  gtk_window_set_application(GTK_WINDOW(window), app);

  GObject *button = gtk_builder_get_object(builder, "button1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  button = gtk_builder_get_object(builder, "button2");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  button = gtk_builder_get_object(builder, "quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_cb), window);

  gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

  /* We do not need the builder any more */
  g_object_unref(builder);

  inspect_renderer(window);
}

int main(int argc,
         char *argv[])
{
#ifdef GTK_SRCDIR
  g_chdir(GTK_SRCDIR);
#endif
  g_unsetenv("GDK_BACKEND");

  GtkApplication *app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
