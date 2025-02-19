#version 330 core

struct ExampleObject
{
    vec4 color;
    vec3 position;
    float dummy;
};

// The objects uniform buffer needs to be named Objects
layout (std140) uniform Objects
{
    ExampleObject examples[2048];
};

// The vpMatrix uniform buffer needs to be named Matrices
// TODO: rename this??
layout (std140) uniform Matrices
{
    mat4 vpMatrix;
};

out vec3 vColor;

void main() 
{
    Example example = examples[gl_InstanceID];
    gl_Position = vpMatrix * vec4(example.position, 1.0);
    vColor = example.color.rgb;
};
