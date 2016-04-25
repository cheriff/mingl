extern: glfw glm
GOOD_GLFW_REV=d97044d9acc0c76433edda5a75b3196957906ca5
GOOD_GLM_REV=79643cf4fa210ed8a5509243ef4613b555a9441c


GLFW_SRC=extern/code/glfw-src/README.md
GLFW_OBJDIR=extern/code/glfw-obj/Makefile
GLFW_LIB=extern/code/glfw-obj/src/libglfw3.a

glfw: extern/include/GLFW extern/lib/libglfw3.a

extern/code:
	mkdir -p extern/code

$(GLFW_SRC): | extern/code
	git clone https://github.com/glfw/glfw.git extern/code/glfw-src
	(cd extern/code/glfw-src && git reset --hard $(GOOD_GLFW_REV))

$(GLFW_OBJDIR): $(GLFW_SRC)
	mkdir -p extern/code/glfw-obj
	(cd extern/code/glfw-obj && cmake ../glfw-src)

$(GLFW_LIB): $(GLFW_OBJDIR)
	$(MAKE) -C extern/code/glfw-obj glfw


extern/include/GLFW: $(GLFW_SRC)
	mkdir -p extern/include
	cp -R ./extern/code/glfw-src/include/GLFW extern/include/GLFW

extern/lib/libglfw3.a: $(GLFW_LIB) 
	@mkdir -p extern/lib
	cp $< $@



glm: extern/include/glm
GLM_SRC=extern/code/glm-src/readme.md

$(GLM_SRC):
	git clone https://github.com/g-truc/glm.git extern/code/glm-src
	(cd extern/code/glm-src && git reset --hard $(GOOD_GLM_REV))

extern/include/glm: $(GLM_SRC)
	mkdir -p extern/include
	cp -R extern/code/glm-src/glm extern/include

CFLAGS+=-Iextern/include/
LDFLAGS+=-Lextern/lib -lglfw3 \
	-framework Cocoa -framework OpenGL -framework IOKit \
	-framework CoreVideo  -framework Carbon




