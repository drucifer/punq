#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <glib.h>

void count_lines(char *file, GData **count)
{
	gchar *line;
	gsize len, term;
	GError *err = NULL;
	GIOChannel *io = strcmp(file, "-") == 0 ?
		g_io_channel_unix_new(fileno(stdin)) :
		g_io_channel_new_file(file, "r", &err);
	while (G_IO_STATUS_NORMAL == g_io_channel_read_line (io, &line, &len, &term, &err)) {
		GQuark q = g_quark_from_string(line);
		g_free(line);
		gpointer c = g_datalist_id_get_data(count, q);
		if (c == NULL) {
			c = g_malloc(sizeof(int));
			*(int *)c = 0;
			g_datalist_id_set_data(count, q, c);
		}
		(*(int *)c)++;
	}
}

void print_fn(GQuark q, gpointer c, gpointer ignored) {
	printf("%7d %s", *(int *)c, g_quark_to_string(q));
}

void print_counts(GData **count) {
	g_datalist_foreach(count, print_fn, NULL);
}

int main (int argc, char *argv[]) {
	GData *counts;
	g_datalist_init(&counts);
	for (int i = 1; i < argc; i++) {
		count_lines(argv[i], &counts);
	}
	if (argc == 1) {
		count_lines("-", &counts);
	}

	print_counts(&counts);
}
