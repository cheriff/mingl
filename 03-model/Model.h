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

struct IndexList
{
    size_t size() { return elem_count; }
    size_t buff_sz() { return elem_count*elem_size; }
    void* get() { return list; }
    bool empty()  { return false; }

    IndexList() :
        list(NULL),
        elem_size(0),
        elem_count(0)
    {}

    IndexList(size_t _es, size_t _count) :
        elem_size(_es),
        elem_count(_count)
    {
        list = malloc(elem_size*elem_count);
    }

    void *list;
    size_t elem_size;
    size_t elem_count;

    stride_iter begin() {
        return stride_iter(list, elem_size ,elem_count);
    }
    stride_iter end() {
        return stride_iter(list, elem_size ,0, elem_count);
    }

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

    std::vector<ModelGroup> groups;
    std::vector<ModelLayout> attributes;
    const char *error;
    std::unique_ptr<uint8_t[]> buffer;
};

Model * getDefaultModel(void);
ModelGroup *getGroup(Model *m, const char *name);
ModelLayout *getAttribute(Model *m, const std::string name);
void dumpModel(Model*);

#endif // __MODEL_H__
