TARGETS+=basic_window

basic_window_SRC=$(wildcard 01-basic_window/*.cpp)
basic_window_OBJ=$(patsubst 01-basic_window/%,build/01/%,$(patsubst %.cpp,%.o,$(basic_window_SRC)))

build/01:
	@mkdir -p $@

$(basic_window_OBJ): build/01/%.o : 01-basic_window/%.cpp | build/01
	$(CC) -c $(CFLAGS) -o$@ $<

basic_window: $(basic_window_OBJ)
	$(CC) $(basic_window_OBJ) $(LDFLAGS) -o $@






