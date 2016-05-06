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

build/02/material.o: 02-imgui/Material.h




NUM=02
NAME=imgui
EXTRA_LIBS=-limgui
$(eval $(DOIT))






