#version 330 core

layout(location = 0) in float height;
layout(location = 1) in vec3 color;

uniform vec3 origin;
uniform vec2 dimensions;
uniform ivec2 numPoints;

layout (std140) uniform Matrices
{
    mat4 vpMatrix;
};

out vec3 vColor;

void main()
{
    float x = float(gl_VertexID % numPoints.x);
    float y = float(gl_VertexID) / float(numPoints.x);

    vec2 uvCoord = vec2(x, y) / (vec2(numPoints) - vec2(1.0, 1.0)); // [0, 1] x [0, 1]
    vec2 coord = (uvCoord - vec2(0.5, 0.5)) * dimensions; // Center uvCoord, then scale
    
    vec3 pos = vec3(coord.x, height, coord.y) + origin; // Assume y is up

    gl_Position = vpMatrix * vec4(pos, 1.0);
    vColor = vec3(uvCoord.xy, 1.0) * (height + 2.5) / 3.0;
}