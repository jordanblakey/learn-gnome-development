/* libatwaita-basics-window.c
 *
 * Copyright 2026 invisible
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "libatwaita-basics-window.h"

struct _LibatwaitaBasicsWindow
{
  AdwApplicationWindow parent_instance;

  /* Template widget */
  // GtkHeaderBar   *header_bar;
  AdwLeaflet *leaflet;
  AdwStatusPage *results_page;
  AdwStatusPage *home_page;
};

G_DEFINE_FINAL_TYPE (LibatwaitaBasicsWindow, libatwaita_basics_window, ADW_TYPE_APPLICATION_WINDOW)

static void
libatwaita_basics_window_class_init (LibatwaitaBasicsWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (
      widget_class,
      "/org/jordanblakey/LibatwaitaBasics/libatwaita-basics-window.ui");
  // gtk_widget_class_bind_template_child(widget_class, LibatwaitaBasicsWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, LibatwaitaBasicsWindow, leaflet);
  gtk_widget_class_bind_template_child (widget_class, LibatwaitaBasicsWindow, results_page);
  gtk_widget_class_bind_template_child (widget_class, LibatwaitaBasicsWindow, home_page);
}

enum Options
{
  ROCK,
  PAPER,
  SCISSORS
};

enum Results
{
  VICTORY,
  DEFEAT,
  DRAW
};

static int
get_result (int player_choice, int random_choice)
{
  if (player_choice == random_choice)
    {
      return DRAW;
    }
  else if ((player_choice == ROCK & random_choice == SCISSORS) ||
           (player_choice == PAPER & random_choice == ROCK) ||
           (player_choice == SCISSORS & random_choice == PAPER))
    {
      return VICTORY;
    }
  else
    {
      return DEFEAT;
    }
}

static void
show_result (LibatwaitaBasicsWindow *self, int choice)
{
  int random_choice = g_random_int_range (0, SCISSORS + 1);
  int result = get_result (choice, random_choice);

  adw_leaflet_set_visible_child (self->leaflet, GTK_WIDGET (self->results_page));

  if (random_choice == ROCK)
    {
      adw_status_page_set_icon_name (self->results_page, "mountain-symbolic");
    }
  else if (random_choice == PAPER)
    {
      adw_status_page_set_icon_name (self->results_page, "document-text-symbolic");
    }
  else if (random_choice == SCISSORS)
    {
      adw_status_page_set_icon_name (self->results_page, "cut-symbolic");
    }

  if (result == VICTORY)
    {
      adw_status_page_set_title (self->results_page, "You Won!");
    }
  else if (result == DRAW)
    {
      adw_status_page_set_title (self->results_page, "Draw!");
    }
  else if (result == DEFEAT)
    {
      adw_status_page_set_title (self->results_page, "Defeat!");
    }
}

static void
on_rock_picked (LibatwaitaBasicsWindow *self)
{
  g_print ("picked rock");
  show_result (self, ROCK);
}

static void
on_scissors_picked (LibatwaitaBasicsWindow *self)
{
  g_print ("picked scissors");
  show_result (self, SCISSORS);
}

static void
on_paper_picked (LibatwaitaBasicsWindow *self)
{
  g_print ("picked paper");
  show_result (self, PAPER);
}

static void
on_replay_activated (LibatwaitaBasicsWindow *self)
{
  g_print ("picked replay");
  if (adw_leaflet_get_visible_child (self->leaflet) == self->results_page) {
    adw_leaflet_set_visible_child (self->leaflet, GTK_WIDGET(self->home_page));
  }
}

static void
register_action (LibatwaitaBasicsWindow *self, gchar *action_name, GCallback handler)
{
  GSimpleAction *action = g_simple_action_new (action_name, NULL);
  g_signal_connect_swapped (action, "activate", handler, self);
  g_action_map_add_action (G_ACTION_MAP (self), G_ACTION (action));
}

static void
libatwaita_basics_window_init (LibatwaitaBasicsWindow *self)
{
  gtk_widget_set_size_request (GTK_WIDGET (self), 400, 600);
  gtk_widget_init_template (GTK_WIDGET (self));
  register_action (self, "rock", G_CALLBACK (on_rock_picked));
  register_action (self, "paper", G_CALLBACK (on_paper_picked));
  register_action (self, "scissors", G_CALLBACK (on_scissors_picked));
  register_action (self, "replay", G_CALLBACK (on_replay_activated));
}

