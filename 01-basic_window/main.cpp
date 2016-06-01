#include <stdio.h>

#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define catchGlError() \
    do { int errcount = 0;\
         GLenum err;\
         while((err = glGetError()) != GL_NO_ERROR) {errcount++; printf("GLERROR @%d: %x\n", __LINE__, err); }\
         if (errcount)  assert(!"GL Errors. Aborting");\
    } while (0);

struct global {
    struct { int width, height; } framebuffer;
    struct { int width, height; } window;
    glm::mat4 projection;
};

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
    glfwSetWindowShouldClose(window, 1);
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

        const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
        const GLubyte* version = glGetString (GL_VERSION); // version as a string
        printf("Renderer : %s\n", renderer);
        printf("OpenGL   : %s\n", version);
        printf("Shader   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("GLFW     : %s\n", glfwGetVersionString());

        glfwSwapInterval(1);
    }

    {
        int width, height;

        glfwGetWindowSize(window, &width, &height);
        window_size(window, width, height);

        glfwGetFramebufferSize(window, &width, &height);
        framebuffer_size(window, width, height);
    }

    GLuint shader;
    int position_index, colour_index;
    int projection_index, modelview_index;
    {
        #include "shader.h"
        GLuint vsh;
        {
            vsh = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vsh, 1, &vertex_shader, NULL);
            glCompileShader(vsh);

            int is_ok;
            glGetShaderiv(vsh, GL_COMPILE_STATUS, &is_ok);
            assert(is_ok == GL_TRUE && "Failed to compile vertex shader");
        }
        GLuint fsh;
        {
            fsh = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fsh, 1, &fragment_shader, NULL);
            glCompileShader(fsh);

            int is_ok;
            glGetShaderiv(fsh, GL_COMPILE_STATUS, &is_ok);
            assert(is_ok == GL_TRUE && "Failed to compile fragment shader");
        }

        shader = glCreateProgram ();
        glAttachShader(shader, vsh);
        glAttachShader(shader, fsh);
        glLinkProgram(shader);

        int is_ok = 42;
        glGetProgramiv(shader, GL_LINK_STATUS, &is_ok);
        assert(is_ok == GL_TRUE && "Failed to link shader");

        position_index = glGetAttribLocation(shader, "position");
        assert(position_index != -1);

        colour_index = glGetAttribLocation(shader, "colour");
        assert(colour_index != -1);

        projection_index = glGetUniformLocation(shader, "projection");
        assert(projection_index != -1);

        modelview_index = glGetUniformLocation(shader, "modelview");
        assert(modelview_index != -1);
    }
    catchGlError();

    GLuint vao;
    int num_triangles;
    {
        GLuint vbo;
        #include "tri_geometry.h"
        glGenBuffers(1, &vbo);
        glBindBuffer (GL_ARRAY_BUFFER, vbo);
        glBufferData (GL_ARRAY_BUFFER, triangle_geometry.data_size, triangle_geometry.data, GL_STATIC_DRAW);
        num_triangles = triangle_geometry.num_triangles;

        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);
        glBindBuffer (GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(position_index);
        glVertexAttribPointer(position_index, 4, GL_FLOAT, 0, 0, (void*)0);

        uintptr_t offset_to_colours = sizeof(float)*num_triangles*4*3;
        glEnableVertexAttribArray(colour_index);
        glVertexAttribPointer(colour_index  , 4, GL_FLOAT, 0, 0, (void*)offset_to_colours );
    }
    catchGlError();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    catchGlError();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto translate_vec = glm::vec3(0,0, -2);
        auto rotate_vec = glm::vec3(0,1,0);
        static float rot = 0;

        auto modelview = glm::mat4(1);
        modelview = glm::translate(modelview, translate_vec);
        modelview = glm::rotate   (modelview, rot+=0.05f, rotate_vec);


        glUseProgram(shader);
        glBindVertexArray(vao);

        glUniformMatrix4fv(projection_index,
                1, GL_FALSE, glm::value_ptr(globals.projection));
        glUniformMatrix4fv(modelview_index,
                1, GL_FALSE, glm::value_ptr(modelview));

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
        catchGlError();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Goodbye\n");

    return 0;
}
