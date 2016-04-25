
static char * vertex_shader = (char *)
    "#version 410\n"
    "in vec4 position;\n"
    "in vec4 colour;\n"
    "\n"
    "uniform mat4 modelview, projection;\n"
    "\n"
    "out vec4 vertex_colour;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec4 pos_eye = modelview * position;\n"
    "   gl_Position = projection * pos_eye;\n"
    "   vertex_colour = colour;\n"
    "}\n";



static char * fragment_shader = (char *)
    "#version 410\n"
    "in vec4 vertex_colour;\n"
    "out vec4 fragment_colour;\n"
    "void main()\n"
    "{\n"
    "   fragment_colour =  vertex_colour;\n"
    "}\n";
