C99?=c99
CFLAGS=-Werror -Wall -O3

override CFLAGS+=-g `pkg-config --cflags glib-2.0`
override LDFLAGS+=`pkg-config --libs glib-2.0`

CC=$(C99)

all: punq

punq: punq.o

.PHONY: clean

clean:
	-rm -f punq punq.o
