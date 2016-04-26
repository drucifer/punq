all: punq


punq: punq.c
	gcc -std=c11 -o $@ $< `pkg-config --cflags --libs glib-2.0`

.PHONY: clean

clean:
	-rm -f punq
