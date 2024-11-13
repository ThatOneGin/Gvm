CC = gcc
CFLAGS = -Wall -Wextra

all: gasm grt

grt: gvm.c grt.c gvm.h
	$(CC) -o grt grt.c $(CFLAGS)

gasm: gasm.c gvm.c gvm.h
	$(CC) -o gasm gasm.c $(CFLAGS)