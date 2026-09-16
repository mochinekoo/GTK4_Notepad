#include <filesystem>
#include <fstream>
#include <iostream>
#include <gtk/gtk.h>
#include <string>

namespace {
    const int DEFAULT_WIDTH = 300, DEFAULT_HEIGHT = 300;
    GtkApplication* gtkApplication_ = nullptr;
    GtkWindow* gtkMainWindow_ = nullptr;
    GtkWidget* mainWindowWidget_ = nullptr;
    GtkTextBuffer* textBuffer_ = nullptr;

    enum class OpenFileDialogType {
        OPEN,
        SAVE
    };
}

static void activate(GtkApplication* app, gpointer data);
static void onClickMenu(GtkWidget* widget, gpointer data);
static void onEvent(GtkWidget* widget, gpointer data);
static void openFileDialog();

// TIP コードを<b>Run</b>するには、<shortcut actionId="Run"/> を押すか、ガターにある <icon src="AllIcons.Actions.Execute"/> アイコンをクリックします。

int main(int argc, char** argv) {
    gtkApplication_ = gtk_application_new("mochineko.gtk4.notepad", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(gtkApplication_, "activate", G_CALLBACK(activate), nullptr);

    const int status = g_application_run(G_APPLICATION(gtkApplication_), argc, argv);
    g_object_unref(gtkApplication_);

    return status;
}

static void activate(GtkApplication* app, gpointer data) {
    mainWindowWidget_ = gtk_application_window_new(app);
    gtkMainWindow_ = GTK_WINDOW(mainWindowWidget_);

    gtk_window_set_title(gtkMainWindow_, "メモ帳");
    gtk_window_set_default_size(gtkMainWindow_, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    auto* actionGroup = g_simple_action_group_new();
    auto* openFileAction = g_simple_action_new("openFile", nullptr);
    auto* saveAction = g_simple_action_new("save", nullptr);
    auto* closeAction = g_simple_action_new("close", nullptr);
    g_signal_connect(openFileAction, "activate", G_CALLBACK(onClickMenu), nullptr);
    g_signal_connect(saveAction, "activate", G_CALLBACK(onClickMenu), nullptr);
    g_signal_connect(closeAction, "activate", G_CALLBACK(onClickMenu), nullptr);
    g_action_map_add_action(G_ACTION_MAP(actionGroup), G_ACTION(openFileAction));
    g_action_map_add_action(G_ACTION_MAP(actionGroup), G_ACTION(saveAction));
    g_action_map_add_action(G_ACTION_MAP(actionGroup), G_ACTION(closeAction));
    gtk_widget_insert_action_group(mainWindowWidget_, "menu", G_ACTION_GROUP(actionGroup));

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GMenu* menu = g_menu_new();
    GMenu* fileMenu = g_menu_new();
    g_menu_append(fileMenu, "開く", "menu.openFile");
    g_menu_append(fileMenu, "保存", "menu.save");
    g_menu_append(fileMenu, "閉じる", "menu.close");

    g_menu_append_submenu(menu, "ファイル", G_MENU_MODEL(fileMenu));
    GtkWidget* menuBar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menu));

    gtk_box_append(GTK_BOX(vbox), menuBar);

    GtkWidget* textView = gtk_text_view_new();
    textBuffer_ = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));

    g_signal_connect(textBuffer_, "insert-text", G_CALLBACK(onEvent), nullptr);
    gtk_box_append(GTK_BOX(vbox), textView);

    gtk_window_set_child(gtkMainWindow_, vbox);

    gtk_window_present(gtkMainWindow_);
}

void openFileDialog(OpenFileDialogType type) {
    GtkFileDialog* fileDialog = gtk_file_dialog_new();

    GListStore* listStore = g_list_store_new(GTK_TYPE_FILE_FILTER);
    GtkFileFilter* fileFilter = gtk_file_filter_new();
    gtk_file_filter_set_name(fileFilter, "テキストファイル");
    gtk_file_filter_add_suffix(fileFilter, "txt");

    g_list_store_append(listStore, fileFilter);

    gtk_file_dialog_set_filters(fileDialog, G_LIST_MODEL(listStore));
    gtk_file_dialog_open(fileDialog, gtkMainWindow_, nullptr, [](GObject* object, GAsyncResult* result, gpointer data) {
        GtkFileDialog* fileDialog = GTK_FILE_DIALOG(object);
        if (fileDialog == nullptr) return;
        GError* error = nullptr;
        GFile* file = gtk_file_dialog_open_finish(fileDialog, result, &error);
        OpenFileDialogType type = (OpenFileDialogType) GPOINTER_TO_INT(data);

        if (file == nullptr) return;
        char* path = g_file_get_path(file);
        if (type == OpenFileDialogType::OPEN) {

        }
        else if (type == OpenFileDialogType::SAVE) {
            GtkTextIter startItr;
            gtk_text_buffer_get_start_iter(textBuffer_, &startItr);
            GtkTextIter endItr;
            gtk_text_buffer_get_end_iter(textBuffer_, &endItr);
            char* text = gtk_text_buffer_get_text(textBuffer_, &startItr, &endItr, false);
            //g_print(text);
            std::ofstream outputFile(path);
            outputFile << text;
            g_print("Saved\n");
        }

    }, GINT_TO_POINTER((int)type));
}

void onClickMenu(GtkWidget *widget, gpointer data) {
    auto* action = G_SIMPLE_ACTION(widget);
    if (action == nullptr) return;
    auto* actionName = g_action_get_name(G_ACTION(action));
    if (actionName == nullptr) return;

    if (g_str_equal(actionName, "openFile")) {
        openFileDialog(OpenFileDialogType::OPEN);
    }
    else if (g_str_equal(actionName, "save")) {
        openFileDialog(OpenFileDialogType::SAVE);
    }
    else if (g_str_equal(actionName, "close")) {
        gtk_window_close(gtkMainWindow_);
    }
}

void onEvent(GtkWidget* widget, gpointer data) {
    auto* textView = GTK_TEXT_VIEW(widget);
    auto* textBuffer = GTK_TEXT_BUFFER(widget);
    if (textBuffer != nullptr) {
        gtk_window_set_title(gtkMainWindow_, "メモ帳（*）");
    }
}