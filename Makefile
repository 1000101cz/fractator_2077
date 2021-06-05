CFLAGS+= -Wall -Werror -std=gnu99 -o3
LDFLAGS=-pthread -lm

CFLAGS+=$(shell sdl2-config --cflags)

HW=prgsem
BINARIES=Fractator_2077

LDFLAGS+=$(shell sdl2-config --libs)

all: ${BINARIES}

OBJS=${patsubst %.c,%.o,${wildcard *.c}}

Fractator_2077: ${OBJS}
	gcc ${OBJS} ${LDFLAGS} -o $@

${OBJS}: %.o: %.c
	gcc -c ${CFLAGS} $< -o $@

clean:
	rm -f ${BINARIES} ${OBJS}
	rm -f ${HW}-brute.zip
