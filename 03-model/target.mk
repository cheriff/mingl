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

DEPS += $($(NAME)_OBJ:.o=.d)
build/$(NUM)/%.d : $(NUM)-$(NAME)/%.cpp | build/$(NUM)
	$(CC) $(CFLAGS) -MF"$$@" -MG -MM -MP -MT"$$(@:.d=.o)" "$$<"
endef

model.objb: triangle.obja extern/code/objconv/objconv
	extern/code/objconv/objconv $< $@
	@echo
	@echo
	@echo


ASSETS+=model.objb



NUM=03
NAME=model
EXTRA_LIBS=-limgui
$(eval $(DOIT))






