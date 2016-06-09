#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <fstream>

#include "Model.h"

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

const uint32_t expected_magic = 0x0B1EC701;
const uint16_t expected_major = 0x0001;
const uint16_t expected_minor = 0x0001;

template<typename T>
static inline void
do_read(std::ifstream &fp, T &val)
{
    auto writeptr = reinterpret_cast<char *>(&val);
    fp.read(writeptr, sizeof(val));
}

struct FileHeader {
    uint32_t magic;
    uint16_t major;
    uint16_t minor;

    uint16_t index_type;
    uint16_t index_count;
    uint32_t buffsize;
    char     name[16];
    uint32_t num_attrs;
    uint32_t num_groups;
};

struct FileAttr {
    char     name[16];
    uint32_t stride;
    uint32_t offset;
    uint32_t elem_type;
    uint32_t elem_count;
};

struct FileGroup {
    char     name[16];
    uint16_t base;
    uint16_t count;
};

#if 0
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_DOUBLE                         0x140A
#endif

const size_t sizeof_gltype(int type)
{
    switch(type) {
        case(GL_BYTE)           : return sizeof(int8_t);
        case(GL_UNSIGNED_BYTE)  : return sizeof(uint8_t);
        case(GL_SHORT)          : return sizeof(int16_t);
        case(GL_UNSIGNED_SHORT) : return sizeof(uint16_t);
        case(GL_INT)            : return sizeof(int32_t);
        case(GL_UNSIGNED_INT)   : return sizeof(uint32_t);
        case(GL_FLOAT)          : return sizeof(float);
        case(GL_DOUBLE)         : return sizeof(double);
    }
    assert(!"Unknown type");
}




Model::Model(std::ifstream &fin, const std::string name)
{
    fin.seekg (0, std::ios::end);
    int fsize = fin.tellg();
    fin.seekg (0, std::ios::beg);

    printf("Fils is %d big\n", fsize);
    if (fsize < sizeof(FileHeader)) {
        error = "File too small (less than header)";
        return;
    }

    FileHeader header;
    do_read(fin, header);

    if (header.magic != expected_magic) {
        error = "Incorrect Magic";
        return;
    }

    if (header.major != expected_major) {
        error = "Incorrect file version major";
        return;
    }

    if (header.minor < expected_minor) {
        error = "File version minor too small";
        return;
    }

    printf("Loaded %s. Has %d attrs, %d groups\n",
            header.name,
            header.num_attrs, header.num_groups);
    printf("Has %d indices of typs %x\n",
            header.index_count, header.index_type);

    printf("Buffsize = %d\n", header.buffsize);

    const size_t expected_size = sizeof(header) +
                                 header.num_attrs   * sizeof(FileAttr)                 +
                                 header.num_groups  * sizeof(FileGroup)                +
                                 header.index_count * sizeof_gltype(header.index_type) +
                                 header.buffsize;
    if (fsize < expected_size) {
        printf("is %d big, expected %d\n", (int)fsize, (int)expected_size);
        // File is too small to contain what the header promises it does.
        error = "File too small";
        return;
    }

    if (fsize > expected_size) {
        // Stray bytes might be benign,, but indicates something odd is afoot.
        // Panic for now.
        error = "File larger than expected";
        return;
    }
    printf("Assume file is %zu\n", expected_size);

    for (int i=0; i<header.num_attrs; i++) {
        FileAttr attr;
        do_read(fin, attr);
        attributes.push_back(ModelLayout(attr.name,
                    attr.offset, attr.stride,
                    attr.elem_type, attr.elem_count));

        printf("Attr: %d : %s\n", i, attr.name);
        printf("    Element type %x, count %d\n",
                attr.elem_type, attr.elem_count);
        printf("    Offset %d\n", attr.offset);
        printf("    Stride %d\n", attr.stride);
    }

    for (int i=0; i<header.num_groups; i++) {
        FileGroup group;
        do_read(fin, group);
        groups.push_back(ModelGroup(group.name, group.base, group.count));

        printf("Group: %d : %s\n", i, group.name);
        printf("    Index base: %d\n", group.base);
        printf("    Index count: %d\n", group.count);
    }

    IndexList indices;
    if (header.index_type == GL_UNSIGNED_BYTE) {
        indices = IndexList(sizeof(uint8_t), header.index_count);
    }
    else if (header.index_type == GL_UNSIGNED_SHORT) {
        indices = IndexList(sizeof(uint16_t), header.index_count);
    }
    else if (header.index_type == GL_UNSIGNED_INT) {
        indices = IndexList(sizeof(uint32_t), header.index_count);
    }

    fin.read(reinterpret_cast<char *>(indices.get()), indices.buff_sz());


    printf("Indices:\n");
    for(auto a : indices) {
        printf("%d\n", a);

    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);

    printf("Loaded %s\n", name.c_str());
    error = NULL;
}

