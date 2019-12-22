# Makefile ImagIn

CC = gcc
CFLAGS = -W -Wall -Wextra -std=c99 -Werror -O3
CPPFLAGS =
LDFLAGS = -lm

# libraries
SDL = `pkg-config --cflags --libs sdl` -lSDL_image
GTK = `pkg-config --cflags --libs gtk+-3.0`
LM = -lm

SRC = src/imagin.c

all: imagin tmp

imagin: $(SRC) src/imagin.c
	$(CC) -o $@.out $(SRC) $(CFLAGS) $(CPPFLAGS) $(SDL) $(GTK) $(LDFLAGS)

tmp:
	mkdir -p tmp

.PHONY: clean

clean:
	${RM} imagin.out
	${RM} imagin.d
	if [ -d tmp ]; then rm -r tmp; fi

# END
