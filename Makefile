# Makefile ImagIn

CC = gcc
CFLAGS = -W -Wall -Wextra -std=c99 -Werror -O3
CPPFLAGS =
LDFLAGS = -lm

# libraries
SDL = `pkg-config --cflags --libs sdl` -lSDL_image
GTK = `pkg-config --cflags --libs gtk+-3.0`
LM = -lm
LIBTIFF = `pkg-config --cflags --libs libtiff-4`
LIBJPEG = `pkg-config --cflags --libs libjpeg`

SRC = src/imagin.c src/import_export/*.c src/modules/*.c

all: imagin tmp

imagin: $(SRC) src/imagin.c
	$(CC) -o $@.out $(SRC) $(CFLAGS) $(CPPFLAGS) $(SDL) $(GTK) $(LIBTIFF) $(LIBJPEG) $(LDFLAGS)

debug:CFLAGS=-g
debug:all

tmp:
	mkdir -p tmp

.PHONY: clean

clean:
	${RM} imagin.out
	${RM} imagin.d
	if [ -d tmp ]; then rm -r tmp; fi

# END
