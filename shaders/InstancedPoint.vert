#version 330 core

struct Point
{
    vec4 color;
    vec3 position;
    float pointSize;
};

layout (std140) uniform Objects
{
    Point points[2048];
};

layout (std140) uniform Matrices
{
    mat4 vpMatrix;
};

out vec3 vColor;

void main() 
{
    Point point = points[gl_InstanceID];
    gl_Position = vpMatrix * vec4(point.position, 1.0);
    gl_PointSize = point.pointSize;
    vColor = point.color.rgb;
};
