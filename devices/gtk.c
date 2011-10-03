/*
 * gtk.c
 *
 *  Created on: Oct 2, 2011
 *      Author: francisco.prates
 */

#include <faclib.h>
#include "gtk.h"

struct s_gtk {
    void *lib;
    GtkWidget *window;
    struct ils_gtk fnc;
} gtk;

static void ini_gtk_fnc(struct s_gtk *gtk)
{
    gtk->fnc.gtk_init = fac_ret_proc_lib(gtk->lib, "gtk_init");
    gtk->fnc.gtk_main = fac_ret_proc_lib(gtk->lib, "gtk_main");
    gtk->fnc.gtk_window_new = fac_ret_proc_lib(gtk->lib, "gtk_window_new");
    gtk->fnc.gtk_widget_show = fac_ret_proc_lib(gtk->lib, "gtk_widget_show");
    gtk->fnc.g_signal_connect_data = fac_ret_proc_lib(gtk->lib,
            "g_signal_connect_data");
    gtk->fnc.gtk_main_quit = fac_ret_proc_lib(gtk->lib, "gtk_main_quit");

}

int _ini_gtk(struct ils_config *config)
{
    gtk.lib = fac_ld_lib(ILS_GTK);
    if (gtk.lib == NULL)
        return -1;

    ini_gtk_fnc(&gtk);

    gtk.fnc.gtk_init(config->argc, config->args);
    gtk.window = gtk.fnc.gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk.fnc.gtk_widget_show(gtk.window);
    gtk.fnc.g_signal_connect(gtk.window, "destroy",
            G_CALLBACK(gtk.fnc.gtk_main_quit), NULL);
    gtk.fnc.gtk_main();

    printf("i: gtk inicializado.\n");

    return 0;
}
