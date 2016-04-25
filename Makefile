all: mingle

CFLAGS=-O3 -Wall -Werror
LDFLAGS=

include external_code.mk

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%,build/%,$(patsubst %.cpp,%.o,$(SRC)))

mingle: $(OBJ) $(LIBS)
	$(CC) $(OBJ) $(LDFLAGS) -o $@


build:
	mkdir build

$(OBJ): build/%.o : src/%.cpp | build extern
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) mingle
deepclean: clean
	rm -rf extern
