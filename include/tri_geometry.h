#define NUM_TRI   1
#define NUM_VERT  (NUM_TRI*3)        // 3 vertices per triangle
#define NUM_FLOAT (NUM_VERT*4*2)       // 4 floats ver vertex (vec4 position, vec4 color)
#define NUM_BYTES (NUM_FLOAT*sizeof(float))

static struct {
    const int num_triangles;
    const int data_size;
    float data[NUM_FLOAT];
} triangle_geometry =
{
    .num_triangles = NUM_TRI,
    .data_size = NUM_BYTES,
    .data = {
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    },
};

