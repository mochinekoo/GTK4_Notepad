#include <iostream>
#include <gtk/gtk.h>

namespace {
    GtkApplication* gtkApplication_ = nullptr;
}

static void activate(GtkApplication* app, gpointer data);

// TIP コードを<b>Run</b>するには、<shortcut actionId="Run"/> を押すか、ガターにある <icon src="AllIcons.Actions.Execute"/> アイコンをクリックします。

int main(int argc, char** argv) {
    gtkApplication_ = gtk_application_new("mochineko.gtk4.notepad", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(gtkApplication_, "activate", G_CALLBACK(activate), nullptr);

    const int status = g_application_run(G_APPLICATION(gtkApplication_), argc, argv);
    g_object_unref(gtkApplication_);

    return status;
}

static void activate(GtkApplication* app, gpointer data) {
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "メモ帳");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    gtk_window_present(GTK_WINDOW(window));
}