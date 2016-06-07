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
    int triangle_base = 0;

    groups.push_back(ModelGroup("MainGroup", num_triangles, triangle_base));

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
    std::cout << "Model: " << name << std::endl;
    std::cout << " VAO: " << vao << std::endl;
    std::cout << " VBO: " << vbo << std::endl;
    std::cout << " ATTRS: " << attributes.size() << std::endl;
    for (auto a: attributes) {
        std::cout << " Attr: " << a.name << std::endl;
        std::cout << "    offset: " << a.offset << std::endl;
        std::cout << "    stride: " << a.stride << std::endl;
        std::cout << "    elem_type: " << a.elem_type << std::endl;
        std::cout << "    elem_count: " << a.elem_count << std::endl;
    }
    std::cout << " GROUPS: " << groups.size() << std::endl;
    for (auto g: groups) {
        std::cout << " Group: " << g.name << std::endl;
        std::cout << "    num_triangles: " << g.num_triangles << std::endl;
        std::cout << "    base: " << g.base << std::endl;
    }

}
