#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include <stride_iterator.h>

struct ModelGroup {
    ModelGroup(const std::string name, const int num_t, const int base_t) :
        name(name), num_triangles(num_t), base(base_t) {}

    const std::string name;
    int num_triangles;
    int base;
};

struct ModelLayout {
    ModelLayout(const std::string name,
            const unsigned int offset, const unsigned int stride,
            const int elem_type, const int elem_count):
                name(name), offset(offset), stride(stride),
                elem_type(elem_type), elem_count(elem_count)
                    {}
            
    const std::string name;
    unsigned int offset;
    unsigned int stride;
    int elem_type;
    int elem_count;
};

struct Model {
    // create default model (colourful triangle0
    Model();
    Model(std::ifstream &fin, const std::string name="NoName");
    void dump();

    const std::string name;
    unsigned int vao, vbo;

    ModelGroup group(const std::string g_name) {
        for(auto g : groups) 
            if (g.name == g_name) return g;
        throw "No groupname";
    }

    ModelLayout attribute(const std::string a_name) {
        for(auto attr : attributes) {
            if (attr.name == a_name) return attr;
        }
        printf("No such for %s\n", a_name.c_str());
        throw "No attrname";
    }

    void DumpData(const ModelLayout &layout, int num=3);

    size_t buffsize;
    std::vector<ModelGroup> groups;
    std::vector<ModelLayout> attributes;
    const char *error;
    std::unique_ptr<uint8_t[]> buffer;
    std::unique_ptr<uint8_t[]> indices;

};

Model * getDefaultModel(void);
ModelGroup *getGroup(Model *m, const char *name);
ModelLayout *getAttribute(Model *m, const std::string name);
void dumpModel(Model*);

#endif // __MODEL_H__
