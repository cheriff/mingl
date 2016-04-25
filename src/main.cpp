#include <stdio.h>

#include <GLFW/glfw3.h>

int
main(int argc, char *argv[])
{
    if (!glfwInit()) {
        return -1;
    }
    printf("Hello World\n");

    return 0;
}
