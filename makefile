CC = gcc
CFLAGS = -ggdb -Wall -Wextra -pedantic
CFILES = $(patsubst %, ../%, $(wildcard src/*.c))
BIN = gvm

all: build

build: build-o
	$(CC) -o $(BIN) build/*.o $(CFLAGS)

build-o:
	cd build; \
	$(CC) $(CFILES) -c $(CFLAGS); \
	cd ..

clean:
	rm $(BIN); \
	rm build/*.o
