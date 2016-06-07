all: apps



CC=g++
CFLAGS=-O3 -Wall -Werror --std=c++11 -Iinclude -Iextern/include
LDFLAGS=

include external_code.mk
include $(wildcard ??-*/target.mk)

-include $(DEPS)


ALL_SRC=$(wildcard */*.cpp)

apps: $(EXTERN) $(TARGETS)

clean:
	rm -rf build
	rm -f $(TARGETS)
	rm -f $(ASSETS)
deepclean: clean
	rm -rf extern

# by default run the target with the higher numbered prefix in its src dir
run: $(lastword $(TARGETS)) $(ASSETS)
	./$<

dd:
	@echo DD is $(DEPS)
