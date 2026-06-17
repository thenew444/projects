
/*
-add some hinden secrets....codes that do funny things/break things
-make it in to a real app that would work on other computers easily (following the gtk4 docs)
-add a calculation history
-add secondary screen that shows the current calculation
-make it possible to get negative numbers
-file headerbar changing color when clicked off of app
-connect keyboard to the buttons
*/


#include "../include/calc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static double   stored_value               = 0.0;
static char     pending_operator           = 0;
static gboolean clear_screen_on_next_digit = FALSE;

int 
main(int   argc, 
     char *argv[])
{
  GtkApplication *app = gtk_application_new ("bad.calculator", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect (app, "startup",  G_CALLBACK (apply_css), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate),  NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

static void 
activate (GtkApplication *app, 
          gpointer        user_data G_GNUC_UNUSED)
{
  GError          *error   = NULL;
  GtkBuilder      *builder = gtk_builder_new ();
  GtkBuilderScope *scope   = gtk_builder_get_scope (builder);

  gtk_builder_cscope_add_callback_symbol (GTK_BUILDER_CSCOPE (scope),
                                         "button_clicked_cb", 
                                          G_CALLBACK (button_clicked_cb));

  if (gtk_builder_add_from_resource (builder, "/bad/calculator/builder.ui", &error) == 0)
    {
      g_printerr ("Error loading UI resource: %s\n", error->message);
      g_clear_error (&error);
      g_object_unref (builder);
      g_application_quit (G_APPLICATION (app));
      return;
    }

  GObject *window = gtk_builder_get_object (builder, "window");
  if (window == NULL)
    {
      g_printerr ("Error: Object definition with id=\"window\" not found in UI resource.\n");
      g_object_unref (builder);
      g_application_quit (G_APPLICATION (app));
      return;
    }

  gtk_window_set_application (GTK_WINDOW (window), app);

  g_object_set_data_full (G_OBJECT (window), "app-builder", builder, g_object_unref);

/*
  GObject *calc_display = gtk_builder_get_object (builder, "calc_display");
  if (calc_display)
    {
      gtk_widget_add_css_class (GTK_WIDGET (calc_display), "calculator-screen");
    }
*/


  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);
}

static void
apply_css (GtkApplication *app       G_GNUC_UNUSED,
           gpointer        user_data G_GNUC_UNUSED)
{
  GtkCssProvider *provider = gtk_css_provider_new ();

  gtk_css_provider_load_from_resource (provider, "/bad/calculator/style.css");

  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref (provider);
}

static GtkLabel *
get_display_from_widget (GtkWidget *widget)
{
  GtkRoot    *root    = gtk_widget_get_root (widget);
  GtkBuilder *builder = g_object_get_data (G_OBJECT (root), "app-builder");

  return GTK_LABEL (gtk_builder_get_object (builder, "calc_display"));
}

void 
button_clicked_cb (GtkButton *btn, 
                   gpointer   user_data G_GNUC_UNUSED)
{
  GtkLabel   *display            = get_display_from_widget (GTK_WIDGET (btn));
  const char *label              = gtk_button_get_label (btn);
  const char *current_text       = gtk_label_get_text (display);
  gboolean    is_negative_number = FALSE;
  
  switch (label[0])
    {
    case 'c':
      clear_screen (display);
      break;

    case '-': 
      printf("fuckoyo %d", atoi(current_text));
      if (atoi(current_text) == 0)
        {
          is_negative_number = TRUE;
          break;
        }
        
    case '/':
    case '*':
    case '+':
      if (stored_value != 0.0 && 
          clear_screen_on_next_digit == FALSE)
        {
          calculate_and_display (display, current_text);
          //update_globals (0.0, 0, TRUE);
        }
      update_globals (atof(current_text), label[0], TRUE);
      break;

    case '=':
      if (stored_value != 0.0)
        {
          calculate_and_display (display, current_text);
          update_globals (0.0, 0, TRUE);
        }
      break;

    default:
      if (clear_screen_on_next_digit == TRUE)
        {
          gtk_label_set_text (display, "0");
          clear_screen_on_next_digit = FALSE;
        }

      if (g_strcmp0 (current_text, "0") == 0) 
        {
          char *negative_label = g_strdup_printf ("%s%s", "-", label);
          if (is_negative_number)
            gtk_label_set_text (display, "?");
          else
            gtk_label_set_text (display, label);

          g_free (negative_label);
        }
      else
        { // Allocate a dynamic string to combine them safely
          char *new_text = g_strdup_printf ("%s%s", current_text, label);

          gtk_label_set_text (display, new_text);
          g_free (new_text); // Free memory to avoid leaks
        }
      break;
    }
}

static void
clear_screen(GtkLabel *screen)
{
  gtk_label_set_text (screen, "0");
  update_globals (0.0, 0, FALSE);
}

static void calculate_and_display (GtkLabel   *display, 
                                   const char *current_text)
{
  double ans = calculate (stored_value, atof (current_text), pending_operator);
  update_globals (0.0, 0, TRUE);
  char new_text[MAX_DIGITS];
  snprintf(new_text, MAX_DIGITS, "%12.7lg", ans);
  gtk_label_set_text (display, new_text);
}

static double 
calculate (double val1, double val2, char operator)
{
  switch (operator)
  {
  case '/':
    if (val1 == 0.0 || val2 == 0.0) 
      return 0.0;
    return val1 / val2;
    break;

  case '*':
    if (val1 == 0.0 || val2 == 0.0) 
      return 0.0;
    return val1 * val2;    
    break;

  case '-':
    return val1 - val2;
    break;

  case '+':
    return val1 + val2;
    break;

  default:
    return -1.0;
    break;
  }
}

static void 
update_globals(double   value, 
               char     operator, 
               gboolean screen)
{
  stored_value               = value;
  pending_operator           = operator;
  clear_screen_on_next_digit = screen;
}