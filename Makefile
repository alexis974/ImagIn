# Makefile ImagIn

CC = gcc
CFLAGS = -W -Wall -Wextra -std=c99 -Werror -O3
CPPFLAGS =
LDFLAGS = -lm

# libraries
LM = -lm
GTK = `pkg-config --cflags --libs gtk+-3.0`
LIBTIFF = `pkg-config --cflags --libs libtiff-4`
LIBJPEG = `pkg-config --cflags --libs libjpeg`
LIBXML = `pkg-config --cflags --libs libxml-2.0`

SRC = src/import_export/*.c src/modules/*.c src/gui/*.c

all: imagin tmp
imagin: $(SRC) src/imagin.c
	$(CC) -o $@.out src/imagin.c $(SRC) $(CFLAGS) $(CPPFLAGS)\
		$(GTK) $(LIBTIFF)\
		$(LIBJPEG) $(LIBXML)\
		$(LDFLAGS)

debug:CFLAGS=-g
debug:all

cli: $(SRC) src/imagin_cli.c tmp
	$(CC) -o imagin_cli.out src/imagin_cli.c $(SRC) $(CFLAGS) $(CPPFLAGS)\
		$(GTK) $(LIBTIFF)\
		$(LIBXML) $(LIBJPEG)\
		$(LDFLAGS)

debug_cli:CFLAGS=-g
debug_cli:cli tmp

tmp:
	mkdir -p tmp

.PHONY: clean

clean:
	${RM} imagin.out
	${RM} imagin_cli.out
	if [ -d tmp ]; then rm -r tmp; fi

# END
