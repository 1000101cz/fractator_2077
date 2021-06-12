CFLAGS+= -Wall -Werror -std=gnu99 -o3
LDFLAGS=-pthread -lm

CFLAGS+=$(shell sdl2-config --cflags)

SOURCES = fractator2077.c events/*.c graphics/*.c system/*.c

HW=prgsem
BINARIES=Fractator_2077

LDFLAGS+=$(shell sdl2-config --libs)

all: ${BINARIES}

OBJS=${patsubst %.c,%.o,${wildcard *.c events/*.c graphics/*.c system/*.c}}

Fractator_2077: ${OBJS}
	gcc ${OBJS} ${LDFLAGS} -o $@

${OBJS}: %.o: %.c
	gcc -c ${CFLAGS} $< -o $@

clean:
	rm -f ${BINARIES} ${OBJS}
