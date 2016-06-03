#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>

struct ModelGroup {
    ModelGroup(const std::string name, const int num_t) :
        name(name), num_triangles(num_t) {}

    const std::string name;
    int num_triangles;
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
    Model();
    void dump();

    const std::string name;
    unsigned int vao, vbo;

    ModelGroup group(const std::string g_name) {
        for(auto g : groups) 
            if (g.name == g_name) return g;
        throw "No groupname";
    }

    ModelLayout attribute(const std::string a_name) {
        for(auto attr : attributes)
            if (attr.name == a_name) return attr;
        throw "No attrname";
    }

    std::vector<ModelGroup> groups;
    std::vector<ModelLayout> attributes;
};

Model * getDefaultModel(void);
ModelGroup *getGroup(Model *m, const char *name);
ModelLayout *getAttribute(Model *m, const std::string name);
void dumpModel(Model*);

#endif // __MODEL_H__
