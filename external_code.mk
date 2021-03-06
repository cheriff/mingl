EXTERN=$(GLFW_DEP) $(GLM_DEP) $(IMGUI_DEP) $(OBJCONV_DEP)

GOOD_GLFW_REV=d97044d9acc0c76433edda5a75b3196957906ca5
GOOD_GLM_REV=79643cf4fa210ed8a5509243ef4613b555a9441c
GOOD_IMGUI_REV=c5149cd53c80d022e6924b20e964544e9537cb20

extern/lib extern/include extern/bin:
	@mkdir -p extern/lib
	@mkdir -p extern/include
	@mkdir -p extern/bin

OBJCONV_DEP=extern/bin/objconv

extern/bin/objconv: extern/code/objconv/objconv | extern/bin
	cp $^ $@

extern/code/objconv/objconv: extern/code/objconv/Makefile
	$(MAKE) -C extern/code/objconv

extern/code/objconv/Makefile:
	git clone github.com:cheriff/objconv extern/code/objconv
	# Don't move to a 'good' version yet, since we're developing off tip for now
	#(cd extern/code/glfw-src && git reset --hard $(GOOD_GLFW_REV))



GLFW_SRC=extern/code/glfw-src/README.md
GLFW_OBJDIR=extern/code/glfw-obj/Makefile
BUILD_GLFW=extern/code/glfw-obj/src/libglfw3.a
LIB_GLFW=extern/lib/libglfw3.a
INC_GLFW=extern/include/GLFW/glfw3.h
GLFW_DEP=$(LIB_GLFW) $(INC_GLFW)


.PHONY: glfw
glfw: extern/include/GLFW extern/lib/libglfw3.a

extern/code:
	mkdir -p extern/code

$(GLFW_SRC): | extern/code
	git clone https://github.com/glfw/glfw.git extern/code/glfw-src
	(cd extern/code/glfw-src && git reset --hard $(GOOD_GLFW_REV))

$(GLFW_OBJDIR): $(GLFW_SRC)
	mkdir -p extern/code/glfw-obj
	(cd extern/code/glfw-obj && cmake ../glfw-src)

$(BUILD_GLFW): $(GLFW_OBJDIR)
	$(MAKE) -C extern/code/glfw-obj glfw

$(INC_GLFW): $(GLFW_SRC) | extern/include
	cp -R ./extern/code/glfw-src/include/GLFW extern/include/GLFW

$(LIB_GLFW): $(BUILD_GLFW)  | extern/lib
	cp $< $@



glm: extern/include/glm
GLM_SRC=extern/code/glm-src/readme.md
GLM_DEP=extern/include/glm

$(GLM_SRC):
	git clone https://github.com/g-truc/glm.git extern/code/glm-src
	(cd extern/code/glm-src && git reset --hard $(GOOD_GLM_REV))

$(GLM_DEP): $(GLM_SRC) | extern/include
	mkdir -p extern/include
	cp -R extern/code/glm-src/glm extern/include

CFLAGS+=-Iextern/include/
LDFLAGS+=-Lextern/lib -lglfw3 \
	-framework Cocoa -framework OpenGL -framework IOKit \
	-framework CoreVideo  -framework Carbon


IMGUI_SRC=extern/code/imgui-src/README.md

extern/code/imgui-src/imgui.h: $(IMGUI_SRC)
extern/code/imgui-src/imgui.cpp: $(IMGUI_SRC)
extern/code/imgui-src/imconfig.h: $(IMGUI_SRC)
extern/code/imgui-src/imgui_draw.cpp: $(IMGUI_SRC)

extern/include/%.h: extern/code/imgui-src/%.h
	cp $< $@

IMGUI_OBJ=extern/code/imgui-src/imgui.o extern/code/imgui-src/imgui_draw.o
$(IMGUI_OBJ): extern/code/imgui-src/%.o: extern/code/imgui-src/%.cpp
	$(CC)  $(CFLAGS) -c $^ -o $@

extern/lib/libimgui.a: $(IMGUI_OBJ)
	$(AR) cr $@ $(IMGUI_OBJ)

IMGUI_DEP=extern/lib/libimgui.a extern/include/imgui.h extern/include/imconfig.h


$(IMGUI_SRC):
	git clone https://github.com/ocornut/imgui.git extern/code/imgui-src
	(cd extern/code/imgui-src && git reset --hard $(GOOD_IMGUI_REV))


