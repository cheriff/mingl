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
        // TODO - better error reporting
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

Material *
getDefaultMaterial(void)
{
    Material *ret = alloc(1, Material);

    #include "shader.h"
    ret->vsh = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    ret->fsh = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
    assert(ret->vsh != 0);
    assert(ret->fsh != 0);

    ret->program = link_shader(ret->vsh, ret->fsh);
    assert(ret->program != 0);


    for(const char *s : { "position", "colour"}) {
        BindPoint bp = {
            .name = strdup(s),
            .location = glGetAttribLocation(ret->program, s),
            .type = 0,
        };
        ret->attributes.push_back(bp);
    }

    for(const char *s : { "projection", "modelview"}) {
        BindPoint bp = {
            .name = strdup(s),
            .location = glGetUniformLocation(ret->program, s),
            .type = 0,
        };
        ret->uniforms.push_back(bp);
    }

    return ret;
}

void
dumpMaterial(Material *mat)
{
    std::for_each(mat->attributes.begin(), mat->attributes.end(), [](BindPoint &bp){
        printf("Attribute  : %s\n", bp.name);
        printf("  Location: %d\n", bp.location);
        printf("  Type    : %d\n", bp.type);
    });
    std::for_each(mat->uniforms.begin(), mat->uniforms.end(), [](BindPoint &bp){
        printf("Uniform  : %s\n", bp.name);
        printf("  Location: %d\n", bp.location);
        printf("  Type    : %d\n", bp.type);
    });
}

BindPoint *
getAttribute(Material *m, const char *name)
{
    for(auto attr = m->attributes.begin(); attr != m->attributes.end(); ++attr) {
        if (strcmp(attr->name, name) == 0) return &(*attr);
    }
    return NULL;
}

BindPoint *
getUniform(Material *m, const char *name)
{
    for(auto uni = m->uniforms.begin(); uni != m->uniforms.end(); ++uni) {
        if (strcmp(uni->name, name) == 0) return &(*uni);
    }
    return NULL;
}
