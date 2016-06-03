#include <vector>
#include <string>

struct BindPoint {
    BindPoint(std::string name, int location, int type):
        name(name), location(location), type(type)
    {}

    std::string name;
    int location;
    unsigned int type;
};

struct Material {
    Material();

    BindPoint uniform(std::string uni_name) {
        for(auto it: uniforms ) {
            if (it.name == uni_name) return it;
        }
        throw "NotFound";
    }

    BindPoint attribute(std::string uni_name) {
        for(auto it: attributes ) {
            if (it.name == uni_name) return it;
        }
        throw "NotFound";
    }
    void dump();

    unsigned int vsh, fsh;
    unsigned int program;
    std::string name;
    std::vector<BindPoint> attributes;
    std::vector<BindPoint> uniforms;
};


