#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Material.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#define alloc(n, type) ((type*)calloc(n, sizeof(type)))

static GLuint
compile_shader(char *text, GLenum shaderType)
{
    GLuint sh = glCreateShader(shaderType);
    assert(sh != 0);

    glShaderSource(sh, 1, &text, NULL);
    glCompileShader(sh);

    int is_ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &is_ok);
    if (is_ok != GL_TRUE) {
        printf("Compile shader failed\n");

        int loglen;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &loglen);
        char *log = (char*)alloca(loglen);

        glGetShaderInfoLog(sh, loglen, NULL, log);
        printf("%s\n", log);



        glDeleteShader(sh);
        return 0;
    }

    return sh;
}

static GLuint
link_shader(GLuint vsh, GLuint fsh)
{
    GLuint ret = glCreateProgram();
    glAttachShader(ret, vsh);
    glAttachShader(ret, fsh);
    glLinkProgram(ret);

    int is_ok = 42;
    glGetProgramiv(ret, GL_LINK_STATUS, &is_ok);
    if (is_ok != GL_TRUE) {
        printf("Link shader failed\n");
        // TODO - better error reporting
        glDetachShader(ret, vsh);
        glDetachShader(ret, fsh);
        glDeleteProgram(ret);
        return 0;

    }

    return ret;
}

Material::Material()
{
    #include "shader.h"
    vsh = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    fsh = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
    assert(vsh != 0);
    assert(fsh != 0);

    program = link_shader(vsh, fsh);
    assert(program != 0);


    for(const char *s : { "position", "colour"}) {
        const int location = glGetAttribLocation(program, s);
        const int type = 0;
        attributes.push_back(BindPoint(s, location, type));
    }

    for(const char *s : { "projection", "modelview"}) {
        const int location = glGetUniformLocation(program, s);
        const int type = 0;
        uniforms.push_back(BindPoint(s, location, type));
    }
    name = "DefaultMaterial";
}

void
Material::dump()
{
    printf("Material: '%s'\n", name.c_str());
    for(auto bp : attributes) {
        printf("  Attribute  : %s\n", bp.name.c_str());
        printf("    Location: %d\n", bp.location);
        printf("    Type    : %d\n", bp.type);
    };

    for (auto bp : uniforms) {
        printf("  Uniform  : %s\n", bp.name.c_str());
        printf("    Location: %d\n", bp.location);
        printf("    Type    : %d\n", bp.type);
    };
}

