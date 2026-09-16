#include <iostream>
#include <gtk/gtk.h>

namespace {
    const int DEFAULT_WIDTH = 300, DEFAULT_HEIGHT = 300;
    GtkApplication* gtkApplication_ = nullptr;
    GtkWindow* gtkMainWindow_ = nullptr;
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
    gtkMainWindow_ = GTK_WINDOW(window);

    gtk_window_set_title(gtkMainWindow_, "メモ帳");
    gtk_window_set_default_size(gtkMainWindow_, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GMenu* menu = g_menu_new();
    GMenu* fileMenu = g_menu_new();
    g_menu_append(fileMenu, "開く", "menu.openFile");
    g_menu_append(fileMenu, "閉じる", "menu.close");

    g_menu_append_submenu(menu, "ファイル", G_MENU_MODEL(fileMenu));
    GtkWidget* menuBar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu));

    gtk_box_append(GTK_BOX(vbox), menuBar);

    gtk_window_set_child(GTK_WINDOW(window), vbox);

    gtk_window_present(GTK_WINDOW(window));
}