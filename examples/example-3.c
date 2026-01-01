#include <gtk/gtk.h>

static cairo_surface_t *surface = NULL;

static void
clear_surface(void)
{
  cairo_t *cr;
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
  cairo_destroy(cr);
}

static void
resize_cb(GtkWidget *widget, int width, int height, gpointer)
{
  if (surface)
  {
    cairo_surface_destroy(surface);
    surface = NULL;
    // g_print("oops");
  }
  GtkNative *native_widget = gtk_widget_get_native(widget);
  if (gtk_native_get_surface(native_widget))
  {
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, gtk_widget_get_width(widget), gtk_widget_get_height(widget));
    clear_surface();
    // g_print("noops?");
  }
}

static void
draw_cb(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data)
{
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);
}

// static void
// draw_brush(GtkWidget *widget, double x, double y)
// {
//   cairo_t *cr;
//   cr = cairo_create(surface);

//   // // draw 1 rect per frame if dragging
//   cairo_rectangle(cr, x - 3, y - 3, 6, 6);
//   cairo_fill(cr);
//   cairo_destroy(cr);

//   gtk_widget_queue_draw(widget);
// }

// static double start_x;
// static double start_y;

// static void
// drag_begin(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
// {
//   start_x = x;
//   start_y = y;

//   draw_brush(area, x, y);
// }

// static void
// drag_update(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
// {
//   static gint64 last_time = 0;
//   gint64 current_time = g_get_monotonic_time();

//   g_print("Interval: %" G_GINT64_FORMAT "us\n", current_time - last_time);
//   last_time = current_time;

//   draw_brush(area, start_x + x, start_y + y);
// }

static void
draw_brush(GtkWidget *widget, double x1, double y1, double x2, double y2)
{
  cairo_t *cr;
  cr = cairo_create(surface);

  // Set brush properties
  cairo_set_line_width(cr, 1.0);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
  cairo_set_source_rgb(cr, 0, 0, 0); // Black ink

  // Draw a line from the last position to the current position
  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_stroke(cr);

  cairo_destroy(cr);

  gtk_widget_queue_draw(widget);
}

static double last_x;
static double last_y;
static double start_x;
static double start_y;

static void
drag_begin(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
  // start_x/y is for the absolute offset, last_x/y is for the segment
  start_x = x;
  start_y = y;
  last_x = x;
  last_y = y;

  // Draw a single dot for the initial click
  draw_brush(area, x, y, x, y);
}

static void
drag_update(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
  double cur_x = start_x + x;
  double cur_y = start_y + y;

  // Draw from the last frame's position to the current frame's position
  draw_brush(area, last_x, last_y, cur_x, cur_y);

  // Store current position for the next frame
  last_x = cur_x;
  last_y = cur_y;
}

static void
drag_end(GtkGestureDrag *gesture, double x, double y, GtkWidget *area)
{
  // draw_brush(area, start_x + x, start_y + y, last_x + x, last_y + y);
}

static void
pressed(GtkGestureClick *gesture, int n_press, double x, double y, GtkWidget *area)
{
  g_print("pressed");
  clear_surface();
  gtk_widget_queue_draw(area);
}

static void
close_window(void)
{
  if (surface)
    cairo_surface_destroy(surface);
}

static void
inspect_renderer(GtkWidget *window)
{
  // Get the surface and the renderer used by the window
  GdkSurface *surface = gtk_native_get_surface(GTK_NATIVE(window));
  GskRenderer *renderer = gtk_native_get_renderer(GTK_NATIVE(window));
  g_print("Current Backend (Windowing Protocol): %s\n", G_OBJECT_TYPE_NAME(gdk_display_get_default()));
  g_print("Graphics Library (Rendering API) in use: %s\n", G_OBJECT_TYPE_NAME(renderer));
}

static void
activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *drawing_area;
  GtkGesture *drag;
  GtkGesture *press;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

  g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

  frame = gtk_frame_new(NULL);
  gtk_window_set_child(GTK_WINDOW(window), frame);

  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 100, 100);

  gtk_frame_set_child(GTK_FRAME(frame), drawing_area);
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_cb, NULL, NULL);

  g_signal_connect_after(drawing_area, "resize", G_CALLBACK(resize_cb), NULL);
  drag = gtk_gesture_drag_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(drag));

  g_signal_connect(drag, "drag-begin", G_CALLBACK(drag_begin), drawing_area);
  g_signal_connect(drag, "drag-update", G_CALLBACK(drag_update), drawing_area);
  g_signal_connect(drag, "drag-end", G_CALLBACK(drag_end), drawing_area);

  press = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(press), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(press));

  g_signal_connect(press, "pressed", G_CALLBACK(pressed), drawing_area);

  gtk_window_present(GTK_WINDOW(window));

  inspect_renderer(window);
}

int main(int argc,
         char **argv)
{
  GtkApplication *app;
  int status;

  // How to print stuff out
  g_print("app pointer: %p\n", (gpointer)app);
  g_print("status: %d\n", status);

  // Prefer wayland and unset env var override
  g_unsetenv("GDK_BACKEND");

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}