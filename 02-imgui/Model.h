#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>

struct ModelGroup {
    const char *name;
    int num_triangles;
};

struct ModelLayout {
    const char *name;
    unsigned int offset;
    unsigned int stride;
    int elem_type;
    int elem_count;
};

struct Model {
    char *name;
    unsigned int vao, vbo;

    std::vector<ModelGroup> groups;
    std::vector<ModelLayout> attributes;
};

Model * getDefaultModel(void);
ModelGroup *getGroup(Model *m, const char *name);
ModelLayout *getAttribute(Model *m, const char *name);
void dumpModel(Model*);

#endif // __MODEL_H__
