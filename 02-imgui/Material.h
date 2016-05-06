#include <vector>

struct BindPoint {
    const char *name;
    int location;
    unsigned int type;
};

struct Material {
    char *name;
    unsigned int vsh, fsh;
    unsigned int program;

    std::vector<BindPoint> attributes;
    std::vector<BindPoint> uniforms;
};

Material * getDefaultMaterial(void);

void dumpMaterial(Material*);




