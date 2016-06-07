define DOIT
TARGETS+=$(NAME)

$(eval $(NAME)_SRC=$(wildcard $(NUM)-$(NAME)/*.cpp))
$(eval $(NAME)_OBJ=$(patsubst $(NUM)-$(NAME)/%,build/$(NUM)/%,$(patsubst %.cpp,%.o,$($(NAME)_SRC))))

build/$(NUM):
	@mkdir -p $$@

$($(NAME)_OBJ): build/$(NUM)/%.o : $(NUM)-$(NAME)/%.cpp | build/$(NUM) $(EXTERN)
	$(CC) -c $(CFLAGS) -o$$@ $$<

$(NAME): $($(NAME)_OBJ) $(EXTERN)
	$(CC) $($(NAME)_OBJ) $(LDFLAGS) -o $$@

DEPS += $($(NAME)_OBJ:.o=.d)
build/$(NUM)/%.d : $(NUM)-$(NAME)/%.cpp | build/$(NUM)
	$(CC) $(CFLAGS) -MF"$$@" -MG -MM -MP -MT"$$(@:.d=.o)" "$$<"

endef


NUM=01
NAME=basic_window
$(eval $(DOIT))






