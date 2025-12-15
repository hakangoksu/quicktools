#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define BASE_DIR "/usr/share/quicktools/scripts"

typedef struct {
    GtkWidget *right_box;
    GtkWidget *window;
} AppWidgets;

/* ===================== HELPERS ===================== */

char *trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return str;
}

/* ===== CATEGORY SORTING STRUCT ===== */

typedef struct {
    char *original_name;
    char *display_name;
    int order;
} CategoryItem;

int extract_order_and_name(const char *dirname, char **clean_name) {
    int i = 0;
    int order = 0;

    while (isdigit((unsigned char)dirname[i])) {
        order = order * 10 + (dirname[i] - '0');
        i++;
    }

    while (dirname[i] == '-' || dirname[i] == '_' || isspace((unsigned char)dirname[i])) {
        i++;
    }

    *clean_name = g_strdup(dirname + i);
    return order;
}

int category_sort_func(const void *a, const void *b) {
    const CategoryItem *ca = *(const CategoryItem **)a;
    const CategoryItem *cb = *(const CategoryItem **)b;
    return ca->order - cb->order;
}

/* ===================== SCRIPT META ===================== */

void get_script_metadata(const char *path, char **name, char **desc) {
    FILE *file = fopen(path, "r");
    *name = NULL;
    *desc = NULL;
    if (!file) return;

    char line[512];
    int check_limit = 10;

    while (fgets(line, sizeof(line), file) && check_limit-- > 0) {
        if (strncmp(line, "# Name:", 7) == 0)
            *name = g_strdup(trim_whitespace(line + 7));
        else if (strncmp(line, "# Description:", 14) == 0)
            *desc = g_strdup(trim_whitespace(line + 14));
    }

    fclose(file);
}

/* ===================== RUN SCRIPT ===================== */

static void run_script(GtkWidget *widget, gpointer data) {
    const char *full_path = (const char *)data;
    const char *term = getenv("TERMINAL");

    char *exec_cmd = g_strdup_printf("bash -c '%s'", full_path);
    char final_cmd[2048];

    if (term && strlen(term) > 0) {
        if (strstr(term, "gnome-terminal"))
            snprintf(final_cmd, sizeof(final_cmd), "%s -- %s", term, exec_cmd);
        else
            snprintf(final_cmd, sizeof(final_cmd), "%s -e %s", term, exec_cmd);
    } else {
        if (g_find_program_in_path("kitty"))
            snprintf(final_cmd, sizeof(final_cmd), "kitty -e %s", exec_cmd);
        else if (g_find_program_in_path("alacritty"))
            snprintf(final_cmd, sizeof(final_cmd), "alacritty -e %s", exec_cmd);
        else if (g_find_program_in_path("konsole"))
            snprintf(final_cmd, sizeof(final_cmd), "konsole -e %s", exec_cmd);
        else if (g_find_program_in_path("xfce4-terminal"))
            snprintf(final_cmd, sizeof(final_cmd), "xfce4-terminal -e %s", exec_cmd);
        else if (g_find_program_in_path("gnome-terminal"))
            snprintf(final_cmd, sizeof(final_cmd), "gnome-terminal -- %s", exec_cmd);
        else if (g_find_program_in_path("xterm"))
            snprintf(final_cmd, sizeof(final_cmd), "xterm -e %s", exec_cmd);
        else {
            GtkAlertDialog *alert = gtk_alert_dialog_new("Terminal Error");
            gtk_alert_dialog_set_detail(alert, "No supported terminal emulator found.");
            gtk_alert_dialog_show(alert, GTK_WINDOW(gtk_widget_get_root(widget)));
            g_free(exec_cmd);
            return;
        }
    }

    g_spawn_command_line_async(final_cmd, NULL);
    g_free(exec_cmd);
}

/* ===================== LOAD SCRIPTS ===================== */

static void load_category_scripts(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = g_object_get_data(G_OBJECT(widget), "app_widgets");
    const char *category_name = (const char *)data;

    GtkWidget *child = gtk_widget_get_first_child(widgets->right_box);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(widgets->right_box), child);
        child = next;
    }

    char cat_path[512];
    snprintf(cat_path, sizeof(cat_path), "%s/%s", BASE_DIR, category_name);

    GtkWidget *header = gtk_label_new(NULL);
    char markup[256];
    snprintf(markup, sizeof(markup),
             "<span size='x-large' weight='bold'>%s</span>", category_name);
    gtk_label_set_markup(GTK_LABEL(header), markup);
    gtk_widget_set_halign(header, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(header, 20);
    gtk_box_append(GTK_BOX(widgets->right_box), header);

    DIR *d = opendir(cat_path);
    if (!d) {
        gtk_box_append(GTK_BOX(widgets->right_box),
                       gtk_label_new("Error opening directory."));
        return;
    }

    struct dirent *dir;
    int count = 0;

    while ((dir = readdir(d)) != NULL) {
        if (!g_str_has_suffix(dir->d_name, ".sh")) continue;
        count++;

        char *full_path = g_strdup_printf("%s/%s", cat_path, dir->d_name);
        char *name = NULL, *desc = NULL;
        get_script_metadata(full_path, &name, &desc);

        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        GtkWidget *text = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        gtk_widget_set_hexpand(text, TRUE);

        GtkWidget *lbl_name = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(lbl_name),
                             g_strdup_printf("<b>%s</b>", name ? name : dir->d_name));
        gtk_widget_set_halign(lbl_name, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(text), lbl_name);

        if (desc) {
            GtkWidget *lbl_desc = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(lbl_desc),
                                 g_strdup_printf("<span size='small' color='gray'>%s</span>", desc));
            gtk_widget_set_halign(lbl_desc, GTK_ALIGN_START);
            gtk_box_append(GTK_BOX(text), lbl_desc);
        }

        GtkWidget *btn = gtk_button_new_with_label("Run");
        g_signal_connect(btn, "clicked", G_CALLBACK(run_script), full_path);

        gtk_box_append(GTK_BOX(row), text);
        gtk_box_append(GTK_BOX(row), btn);
        gtk_box_append(GTK_BOX(widgets->right_box), row);
        gtk_box_append(GTK_BOX(widgets->right_box),
                       gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

        g_free(name);
        g_free(desc);
    }

    closedir(d);

    if (count == 0)
        gtk_box_append(GTK_BOX(widgets->right_box),
                       gtk_label_new("No scripts found."));
}

/* ===================== ACTIVATE ===================== */

static void activate(GtkApplication *app, gpointer user_data) {
    AppWidgets *widgets = g_new0(AppWidgets, 1);

    widgets->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(widgets->window), "QuickTools");
    gtk_window_set_default_size(GTK_WINDOW(widgets->window), 900, 600);

    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_window_set_child(GTK_WINDOW(widgets->window), paned);
    gtk_paned_set_position(GTK_PANED(paned), 250);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *left_scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(left_scroll), left_box);

    GtkWidget *right_scroll = gtk_scrolled_window_new();
    widgets->right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(right_scroll),
                                  widgets->right_box);

    gtk_paned_set_start_child(GTK_PANED(paned), left_scroll);
    gtk_paned_set_end_child(GTK_PANED(paned), right_scroll);

    /* ===== LOAD & SORT CATEGORIES ===== */

    GPtrArray *cats = g_ptr_array_new();
    DIR *d = opendir(BASE_DIR);
    struct dirent *dir;

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type != DT_DIR ||
                !strcmp(dir->d_name, ".") ||
                !strcmp(dir->d_name, ".."))
                continue;

            CategoryItem *c = g_new0(CategoryItem, 1);
            c->original_name = g_strdup(dir->d_name);
            c->order = extract_order_and_name(dir->d_name, &c->display_name);
            g_ptr_array_add(cats, c);
        }
        closedir(d);
    }

    qsort(cats->pdata, cats->len, sizeof(gpointer), category_sort_func);

    for (guint i = 0; i < cats->len; i++) {
        CategoryItem *c = g_ptr_array_index(cats, i);
        GtkWidget *btn = gtk_button_new_with_label(c->display_name);
        g_object_set_data(G_OBJECT(btn), "app_widgets", widgets);
        g_signal_connect(btn, "clicked",
                         G_CALLBACK(load_category_scripts),
                         g_strdup(c->original_name));
        gtk_box_append(GTK_BOX(left_box), btn);
        g_free(c->original_name);
        g_free(c->display_name);
        g_free(c);
    }

    g_ptr_array_free(cats, TRUE);

    gtk_window_present(GTK_WINDOW(widgets->window));
}

/* ===================== MAIN ===================== */

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("org.arch.quicktools", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
