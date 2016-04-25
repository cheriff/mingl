#include <stdio.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

static void
framebuffer_size(GLFWwindow *window, int width, int height)
{
    printf("Window resize: %d x %d\n", width, height);
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
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 480, argv[0], NULL, NULL);
    assert(window != NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size);
    glfwSetKeyCallback(window, key_action);
    glfwMakeContextCurrent(window);

    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf("Renderer : %s\n", renderer);
    printf("OpenGL   : %s\n", version);
    printf("GLFW     : %s\n", glfwGetVersionString());

    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size(window, width, height);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    printf("Goodbye\n");

    return 0;
}
