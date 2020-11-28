#include <glib.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 80

static GHashTable* hash_table;

int cmp_stat_words(gpointer a, gpointer b) {
    gpointer cnt_a = g_hash_table_lookup(hash_table, a);
    gpointer cnt_b = g_hash_table_lookup(hash_table, b);
    if (cnt_b < cnt_a) {
        return -1;
    }
    if (cnt_b > cnt_a) {
        return 1;
    }
    return g_strcmp0(a, b);
}

void iterator(gpointer word, gpointer user_data) {
    if (*(gchar*)word) {
        printf(user_data, word, g_hash_table_lookup(hash_table, word));
    }
}

void key_destroyed(gpointer data) { g_free(data); }

int main() {
    hash_table = g_hash_table_new_full(g_str_hash, g_str_equal,
                                       (GDestroyNotify)key_destroyed, NULL);
    char line[MAX_LINE_LENGTH + 1];
    while (fgets(line, MAX_LINE_LENGTH + 1, stdin)) {
        gchar** v = g_strsplit_set(line, " \t\r\n\v\f", MAX_LINE_LENGTH);
        gchar** p = v;
        while (*p) {
            g_hash_table_insert(
                hash_table, *p,
                GINT_TO_POINTER(
                    (gint)((glong)g_hash_table_lookup(hash_table, *p) + 1)));
            ++p;
        }
        g_free(v);
    }
    GList* keys = g_hash_table_get_keys(hash_table);
    GList* words = g_list_copy(keys);
    words = g_list_sort(words, (GCompareFunc)cmp_stat_words);
    g_list_foreach(words, (GFunc)iterator, "%s: %d\n");

    g_list_free(words);
    g_list_free(keys);
    g_hash_table_destroy(hash_table);
}
