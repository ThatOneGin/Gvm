CC = gcc
CFLAGS = -Wall -Wextra

all: gasm grt

grt:
	$(CC) -o grt grt.c $(CFLAGS)

gasm:
	$(CC) -o gasm gasm.c $(CFLAGS)