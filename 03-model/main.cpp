#include <stdio.h>

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>

#include "globals.h"

#include "imgui.h"
#include "bind_imgui.h"

#include "Material.h"
#include "Model.h"

#define catchGlError() \
    do { int errcount = 0;\
         GLenum err;\
         while((err = glGetError()) != GL_NO_ERROR) {errcount++; printf("GLERROR @%d: %x\n", __LINE__, err); }\
         if (errcount)  assert(!"GL Errors. Aborting");\
    } while (0);



static void 
mouse_button(GLFWwindow *window, int button, int action, int mods)
{
    //printf("Mouse button: %d %d %d\n", button, action, mods);
    auto *globals = (struct global*)glfwGetWindowUserPointer(window);
    assert(globals != NULL);
}

static void
cursor_pos(GLFWwindow *window, double x, double y)
{
    //printf("Cursor pos %f %f\n", x, y);
    auto *globals = (struct global*)glfwGetWindowUserPointer(window);
    assert(globals != NULL);
}

static void 
scroll_wheel(GLFWwindow *window, double x, double y)
{
    //printf("Scroll wheel: %f %f\n", x, y);
}

static void
char_action(GLFWwindow *window, unsigned int c)
{
    auto *globals = (struct global*)glfwGetWindowUserPointer(window);
    assert(globals != NULL);
    //printf("Unicode: %x\n", c);
}

static void
window_size(GLFWwindow *window, int width, int height)
{
    auto *globals = (struct global*)glfwGetWindowUserPointer(window);
    assert(globals != NULL);

    globals->window.width = width;
    globals->window.height = height;
    float aspect = (float)width / (float)height;
    globals->projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
    printf("Win size : %dx%d\n", globals->window.width, globals->window.height);
}

static void
framebuffer_size(GLFWwindow *window, int width, int height)
{
    auto *globals = (struct global*)glfwGetWindowUserPointer(window);
    assert(globals != NULL);

    globals->framebuffer.width = width;
    globals->framebuffer.height = height;
    printf("FB size  : %dx%d\n", globals->framebuffer.width, globals->framebuffer.height);
}


void
key_action(GLFWwindow *window, int,int,int,int)
{
    printf("Key action\n");
}

int
main(int argc, char *argv[])
{
    struct global globals;
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow *window;
    {
        glfwWindowHint(GLFW_DEPTH_BITS, 16);
        glfwWindowHint(GLFW_SAMPLES, 1);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(800, 480, argv[0], NULL, NULL);
        glfwSetWindowUserPointer(window, &globals);
        assert(window != NULL);

        glfwSetFramebufferSizeCallback(window, framebuffer_size);
        glfwSetWindowSizeCallback(window, window_size);
        glfwSetKeyCallback(window, key_action);
        glfwMakeContextCurrent(window);

        glfwSetMouseButtonCallback(window, mouse_button);
        glfwSetCursorPosCallback(window, cursor_pos);
        glfwSetScrollCallback(window, scroll_wheel);
        glfwSetCharCallback(window, char_action);

        const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString (GL_VERSION); // version as a string
        printf("Renderer : %s\n", renderer);
        printf("OpenGL   : %s\n", version);
        printf("Shader   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("GLFW     : %s\n", glfwGetVersionString());

        glfwSwapInterval(1);
    }
    catchGlError();

    globals.glwindow = window;
    ImGui_ImplGlfwGL3_Init(window, /* install_callbacks = */ true);
    catchGlError();

    {
        int width, height;

        glfwGetWindowSize(window, &width, &height);
        window_size(window, width, height);

        glfwGetFramebufferSize(window, &width, &height);
        framebuffer_size(window, width, height);
    }
    catchGlError();

    Material material{};
    catchGlError();

    const int projection_index = material.uniform("projection").location;
    const int modelview_index  = material.uniform("modelview").location;

    const char *modelname = "model.objb";
    std::ifstream model_file(modelname, std::ios::in|std::ios::binary);
    if (!model_file) {
        printf("No such fie: %s\n", modelname);
        return 0;
    }

    auto model = Model(model_file, modelname);
    if (model.error) {
        printf("Load ERROR: %s\n", model.error);
        return 0;
    }
    printf("Load OK\n");

    model.dump();
    catchGlError();

    printf("\n\n");
    auto defmodel = Model();
    defmodel.dump();

    for (auto attr: material.attributes) {
        glEnableVertexAttribArray(attr.location);
        const ModelLayout layout = model.attribute(attr.name);

        glVertexAttribPointer(attr.location,
                layout.elem_count, layout.elem_type,
                0 , //normalised
                layout.stride,
                (void*)(uintptr_t)(layout.offset));

    }
    catchGlError();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    catchGlError();
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplGlfwGL3_NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto translate_vec = glm::vec3(0,0, -2);
        auto rotate_vec = glm::vec3(0,1,0);
        static float rot = 0;

        auto modelview = glm::mat4(1);
        modelview = glm::translate(modelview, translate_vec);
        modelview = glm::rotate   (modelview, rot+=0.05f, rotate_vec);


        glUseProgram(material.program);
        glBindVertexArray(model.vao);
        glUniformMatrix4fv(projection_index,
                1, GL_FALSE, glm::value_ptr(globals.projection));
        glUniformMatrix4fv(modelview_index,
                1, GL_FALSE, glm::value_ptr(modelview));
        glDrawArrays(GL_TRIANGLES, 0, 3);


        ImGui::ShowMetricsWindow();

        ImGui::Render() ;

        glfwSwapBuffers(window);
        glfwPollEvents();
        catchGlError();
    }

    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Goodbye\n");

    return 0;
}
