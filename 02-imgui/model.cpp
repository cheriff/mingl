#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Model.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

#define alloc(n, type) ((type*)calloc(n, sizeof(type)))

Model *
getDefaultModel(void)
{
    #include "tri_geometry.h"
    Model *ret = alloc(1, Model);

    glGenVertexArrays(1, &ret->vao);
    glGenBuffers(1, &ret->vbo);

    glBindVertexArray(ret->vao);
    glBindBuffer (GL_ARRAY_BUFFER, ret->vbo);

    glBufferData (GL_ARRAY_BUFFER,
            triangle_geometry.data_size,
            triangle_geometry.data, GL_STATIC_DRAW);

    int num_triangles = triangle_geometry.num_triangles;

    ModelGroup mg = {
        .name = "MainGroup",
        .num_triangles = num_triangles,
    };
    ret->groups.push_back(mg);


    ModelLayout ml;
    ml = {
        .name = "position",
        .offset = 0,
        .stride = 0,
        .elem_type = GL_FLOAT,
        .elem_count = 4,
    };
    ret->attributes.push_back(ml);

    ml = {
        .name = "colour",
        .offset = sizeof(float)*num_triangles*4*3,
        .stride = 0,
        .elem_type = GL_FLOAT,
        .elem_count = 4,
    };
    ret->attributes.push_back(ml);

    return ret;
}

void
dumpModel(Model *m)
{
}
