#include "../include/system_monitor.h"

int
main (int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new ("system.monitor", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "startup" , G_CALLBACK (apply_css), NULL);
    g_signal_connect (app, "activate", G_CALLBACK (activate) , NULL);

    int status = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref (app);
    return status;
}

static void
apply_css (GtkApplication *app       G_GNUC_UNUSED,
           gpointer        user_data G_GNUC_UNUSED)
{
  GtkCssProvider *provider = gtk_css_provider_new ();

  gtk_css_provider_load_from_resource        (provider, "/system/monitor/style.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref (provider);
}

static void
activate (GtkApplication *app,
          gpointer        user_data G_GNUC_UNUSED)
{
  GError     *error   = NULL;
  GtkBuilder *builder = gtk_builder_new ();
  //GtkBuilderScope *scope = gtk_builder_get_scope (builder);
  //gtk_builder_cscope_add_callback_symbol (GTK_BUILDER_CSCOPE (scope),
  //                                       "button_clicked_cb", 
  //                                        G_CALLBACK (button_clicked_cb));

  if (gtk_builder_add_from_resource (builder, "/system/monitor/builder.ui", &error) == 0)
    {
      g_printerr         ("Error loading UI resource: %s\n", error->message);
      g_clear_error      (&error);
      g_object_unref     (builder);
      g_application_quit (G_APPLICATION (app));
      return;        
    }

  GObject *window = gtk_builder_get_object (builder, "window");
  if (window == NULL)
    {
      g_printerr         ("Error: Object definition with id=\"window\" not found in UI resource.\n");
      g_object_unref     (builder);
      g_application_quit (G_APPLICATION (app));
      return;
    }

  gtk_window_set_application (GTK_WINDOW (window),  app);
  g_object_set_data_full     (G_OBJECT   (window), "app-builder", builder, g_object_unref);
  gtk_window_present         (GTK_WINDOW (window));
}

