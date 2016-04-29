#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <glib.h>

static struct {
	gboolean count;
	gboolean comma;
	gboolean tab;
	gboolean after;
} Punq_options;

static GOptionEntry goptions[] = {
	{ "count", 'c', 0, G_OPTION_ARG_NONE, &Punq_options.count, "Count occurrences of each line", NULL },
	{ "comma", ',', 0, G_OPTION_ARG_NONE, &Punq_options.comma, "Use comma to separate count from text", NULL },
	{ "tab",   't', 0, G_OPTION_ARG_NONE, &Punq_options.tab,   "Use tab to separate count from text", NULL },

	{ NULL }
};

void count_lines(char *file, GHashTable *count) {
	gchar *line;
	gsize len, term;
	GError *err = NULL;
	GIOChannel *io = strcmp(file, "-") == 0 ?
		g_io_channel_unix_new(fileno(stdin)) :
		g_io_channel_new_file(file, "r", &err);
	while (G_IO_STATUS_NORMAL == g_io_channel_read_line(io, &line, &len, &term, &err)) {
		int *c = g_hash_table_lookup(count, line);
		if (!c) {
			c = g_malloc(sizeof(int));
			*c = 0;
			g_hash_table_insert(count, g_strdup(line), c);
		}
		(*c)++;
	}
}

void print_fn(gpointer key, gpointer value, gpointer format) {
	printf(format, *(int *)value, key);
}

void print_counts(GHashTable *count) {
	char *format;
	if (Punq_options.count) {
		if (Punq_options.comma) { 
			format = "%d,%s";
		} else if (Punq_options.tab) {
			format = "%d\t%s";
		} else {
			format = "%7d %s";
		}
	} else {
		format = "%2$s";
	}
	g_hash_table_foreach(count, print_fn, format);
}

int main(int argc, char *argv[]) {
	GError *error = NULL;
	GOptionContext *context;

	context = g_option_context_new("- list unique lines in input files");
	g_option_context_add_main_entries(context, goptions, NULL);
	if (g_option_context_parse(context, &argc, &argv, &error)) {
		g_option_context_free(context);
	} else {
		g_print("%s\n", g_option_context_get_help(context, TRUE, NULL));
		return 1;
	}

	GHashTable *counts = g_hash_table_new(g_str_hash, g_str_equal);
	for (int i = 1; i < argc; i++) {
		count_lines(argv[i], counts);
	}
	if (argc == 1) {
		count_lines("-", counts);
	}

	print_counts(counts);
}
