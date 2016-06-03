
#include <glm/mat4x4.hpp>
struct UI;
struct GLFWwindow;

struct global {
    struct { int width, height; } framebuffer;
    struct { int width, height; } window;
    glm::mat4 projection;
    UI *ui;
    GLFWwindow *glwindow;
};
