#include <gtk/gtk.h>

static void
print_hello(GtkWidget *widget,
            gpointer data)
{
  g_print("Hello World\n");
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  /* create a new window, and set its title */
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");

  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new();

  /* Pack the container in the window */
  gtk_window_set_child(GTK_WINDOW(window), grid);

  button = gtk_button_new_with_label("Button 1");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Button 2");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

  button = gtk_button_new_with_label("Quit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), window);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 2, 1);

  gtk_window_present(GTK_WINDOW(window));

  // DIAGNOSTIC MESSAGES
  g_print("Current Backend (Windowing Protocol): %s\n", G_OBJECT_TYPE_NAME(gdk_display_get_default()));
  /* Get the surface and the renderer used by the window */
  GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window));
  GskRenderer *renderer = gtk_native_get_renderer(GTK_NATIVE(window));
  g_print("Graphics Library (Rendering API) in use: %s\n", G_OBJECT_TYPE_NAME(renderer));
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  // Specify Windowing Protocol - This is not a best practice
  /* Force X11 - Wayland fixes */
  // g_setenv ("GDK_BACKEND", "x11", TRUE); // Force x11
  g_setenv("GDK_BACKEND", "wayland", TRUE); // Force wayland
  // g_setenv ("__GL_MaxFramesAllowed", "1", TRUE); // Forced redraws

  /* * 1. Remove GDK_BACKEND=x11 to allow native Wayland.
   * 2. Set GSK_RENDERER to 'ngl'. This is the modern OpenGL renderer
   * for GTK4 that solves the flickering/trailing issues on Wayland
   * and NVIDIA drivers.
   */
  // g_setenv ("GSK_RENDERER", "gl", TRUE); // New Graphics Library, works with wayland. Tested.
  // g_setenv ("GSK_RENDERER", "vulkan", TRUE);
  // g_setenv ("GSK_RENDERER", "cairo", TRUE); // No GPU, old

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}