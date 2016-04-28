all: apps

CC=g++
CFLAGS=-O3 -Wall -Werror --std=c++11 -Iinclude
LDFLAGS=

include external_code.mk
include $(wildcard ??-*/target.mk)

apps: $(EXTERN) $(TARGETS)

clean:
	rm -rf build
	rm -f $(TARGETS)
deepclean: clean
	rm -rf extern

# by default run the target with the higher numbered prefix in its src dir
run: $(lastword $(TARGETS))
	./$<

