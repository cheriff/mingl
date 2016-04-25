all: mingle

CFLAGS=-O3 -Wall -Werror --std=c++11 -Iinclude
LDFLAGS=

include external_code.mk

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%,build/%,$(patsubst %.cpp,%.o,$(SRC)))

mingle: $(OBJ) $(LIBS)
	$(CC) $(OBJ) $(LDFLAGS) -o $@


build:
	mkdir build

build/main.o: include/shader.h

$(OBJ): build/%.o : src/%.cpp | build extern
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) mingle
deepclean: clean
	rm -rf extern

run: all
	./mingle
