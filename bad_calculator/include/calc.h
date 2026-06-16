#ifndef CALC_H
#define CALC_H

#include <gtk/gtk.h>
#include <glib/gstdio.h>

#define MAX_DIGITS 16

static void activate (GtkApplication *app, gpointer user_data);
static void apply_css (GtkApplication *app G_GNUC_UNUSED, gpointer user_data);
static GtkLabel *get_display_from_widget (GtkWidget *widget);
void button_clicked_cb (GtkButton *btn, gpointer user_data);
static void clear_screen(GtkLabel *screen);
static void calculate_and_display (GtkLabel *display, const char *current_text);
static double calculate (double val1, double val2, const char operator);
static void update_globals(double stored_value, const char pending_operator, gboolean clear_screen);

#endif