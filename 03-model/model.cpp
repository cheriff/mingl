#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Model.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

Model::Model()
{
    #include "tri_geometry.h"
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    glBufferData (GL_ARRAY_BUFFER,
            triangle_geometry.data_size,
            triangle_geometry.data, GL_STATIC_DRAW);

    int num_triangles = triangle_geometry.num_triangles;

    groups.push_back(ModelGroup("MainGroup", num_triangles));

    const std::string pos_name = "position";
    const unsigned int pos_offset = 0;
    const unsigned int pos_stride = 0;
    const int pos_elem_type = GL_FLOAT;
    const int pos_elem_count = 4;
    attributes.push_back(ModelLayout(pos_name, pos_offset, pos_stride, pos_elem_type, pos_elem_count));

    const std::string col_name = "colour";
    const unsigned int col_offset = static_cast<unsigned int>(sizeof(float))*num_triangles*4*3;
    const unsigned int col_stride = 0;
    const int col_elem_type = GL_FLOAT;
    const int col_elem_count = 4;
    attributes.push_back(ModelLayout(col_name, col_offset, col_stride, col_elem_type, col_elem_count));
}

void Model::dump()
{
}
