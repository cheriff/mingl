

struct UI {
    GLuint ShaderHandle;
    GLuint VertHandle, FragHandle;
    GLuint AttribLocationTex, AttribLocationProjMtx;
    GLuint AttribLocationPosition, AttribLocationUV, AttribLocationColor;

    GLuint VboHandle, VaoHandle;
    GLuint ElementsHandle;

    GLuint FontTexture;

    global *globals;
};

UI * ui_init(void *globals);

void ui_startframe(UI *);
void ui_endrame(UI *);

void ui_destroy(UI *ui);

void ui_keycode(UI *ui, unsigned int c);
