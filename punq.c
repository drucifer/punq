#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <glib.h>

void count_lines(char *file, GHashTable *count)
{
	gchar *line;
	gsize len, term;
	GError *err = NULL;
	GIOChannel *io = strcmp(file, "-") == 0 ?
		g_io_channel_unix_new(fileno(stdin)) :
		g_io_channel_new_file(file, "r", &err);
	while (G_IO_STATUS_NORMAL == g_io_channel_read_line (io, &line, &len, &term, &err)) {
		int *c = g_hash_table_lookup(count, line);
		if (!c) {
			c = g_malloc(sizeof(int));
			*c = 0;
			g_hash_table_insert(count, g_strdup(line), c);
		}
		(*c)++;
	}
}

void print_fn(gpointer key, gpointer value, gpointer ignored) {
	printf("%7d %s", *(int *)value, key);
}

void print_counts(GHashTable *count) {
	g_hash_table_foreach(count, print_fn, NULL);
}

int main (int argc, char *argv[]) {
	GHashTable *counts = g_hash_table_new(g_str_hash, (GEqualFunc)g_str_equal);
	for (int i = 1; i < argc; i++) {
		count_lines(argv[i], counts);
	}
	if (argc == 1) {
		count_lines("-", counts);
	}

	print_counts(counts);
}
