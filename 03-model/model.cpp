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
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferData (GL_ARRAY_BUFFER,
            triangle_geometry.data_size,
            triangle_geometry.data, GL_STATIC_DRAW);

    buffsize = triangle_geometry.data_size;

    buffer = std::unique_ptr<uint8_t[]>(new uint8_t[buffsize]);
    memcpy(buffer.get(),
            triangle_geometry.data,
            triangle_geometry.data_size);

    size_t idx_buffsize = triangle_geometry.num_indixes * sizeof(triangle_geometry.indices[0]);
    indices = std::unique_ptr<uint8_t[]>(new uint8_t[idx_buffsize]);
    index_count = triangle_geometry.num_indixes;
    index_type = GL_UNSIGNED_INT;
    memcpy(indices.get(),
            triangle_geometry.indices,
            idx_buffsize);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
            idx_buffsize, indices.get(),
            GL_STATIC_DRAW);

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

    std::cout<< " Buffer Size: " << buffsize << "bytes, ("<<buffsize/4<<" floats)" << std::endl;
    if (1) {
        for (int i = 0; i < buffsize/4; i++) {
            float * fp = &((float*)buffer.get())[i];

            printf("%p {%08lx} :: DEF[%d] = %f\n",
                    fp, (char*)fp - (char*)buffer.get(),
                    i, *fp);
        }
    }

}

void
Model::DumpData(const ModelLayout &layout, int num)
{
    printf("ATTRIB %s :\n", layout.name.c_str());
    printf("\tElemCount %d\n", layout.elem_count);
    printf("\tElemType %d\n", layout.elem_type);
    printf("\tstride %d\n", layout.stride);
    printf("\toffset %d\n", layout.offset);

    uint8_t *ptr = buffer.get() + layout.offset;

    for( int n=0; n<num; n++) {
        printf("%p {%08lx} %d: ", ptr, ptr-buffer.get(), n);

        uint8_t *start = ptr;
        for (int e=0; e<layout.elem_count; e++) {
            switch(layout.elem_type) {
                case GL_FLOAT:
                    printf("%f ", *((float*)ptr));
                    ptr += sizeof(float);
                    break;
                default:
                    printf("??? ");
                    ptr += sizeof(uint32_t);
                    break;
            }
        }
        printf("\n");

        if (layout.stride != 0)
            ptr = start + layout.stride;
    }
    printf("END: %p {%08lx}\n", ptr, ptr-buffer.get());
}


