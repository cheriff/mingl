#include <stdio.h>

#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

int
main(int argc, char *argv[])
{
    if (!glfwInit()) {
        return -1;
    }
    printf("Hello World\n");

    return 0;
}
