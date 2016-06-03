define DOIT
TARGETS+=$(NAME)

$(eval $(NAME)_SRC=$(wildcard $(NUM)-$(NAME)/*.cpp))
$(eval $(NAME)_OBJ=$(patsubst $(NUM)-$(NAME)/%,build/$(NUM)/%,$(patsubst %.cpp,%.o,$($(NAME)_SRC))))

build/$(NUM):
	@mkdir -p $$@

$($(NAME)_OBJ): build/$(NUM)/%.o : $(NUM)-$(NAME)/%.cpp | build/$(NUM) $(EXTERN)
	$(CC) -c $(CFLAGS) -o$$@ $$<

$(NAME): $($(NAME)_OBJ) $(EXTERN)
	$(CC) $($(NAME)_OBJ) $(LDFLAGS) $(EXTRA_LIBS) -o $$@
endef



NUM=03
NAME=model
EXTRA_LIBS=-limgui
$(eval $(DOIT))






