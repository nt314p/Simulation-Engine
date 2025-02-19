#pragma once
// TODO: better name for this file
// will eventually hold all primitive shapes (1d, 2d, 3d)

#include <cglm\cglm.h>
#include "renderer.h"

/* Represents a generic collection of instanced objects.
It uses a vertex buffer to hold vertices, and a 
uniform buffer to hold per instance data
*/
typedef struct InstancedObject
{
    unsigned int MaxObjectCount;
    unsigned int count; // How many objects currently are instantiated
    unsigned int size; // The size of the object in bytes
    unsigned int shaderId;
    unsigned int verticesPerObject;
    GLenum mode; // The mode to render for the draw call (GL_POINTS, GL_LINES, GL_TRIANGLES, etc)
    VertexArray vao;
    UniformBuffer instanceDataBuffer;
    void* objects;
} InstancedObject;

typedef struct Circle
{
    vec3 color;
    float padding;
    vec2 position;
    float radius;
    float padding2;
} Circle;

typedef struct Rect
{
    vec3 color;
    float padding;
    vec2 position;
    float width;
    float height;
} Rect;

typedef struct Line2D
{
    vec3 color;
    float padding;
    vec2 a;
    vec2 b;
} Line2D;

typedef struct Line
{
    vec3 color;
    float padding;
    vec3 a;
    float thickness; // TODO: implement thickness
    vec3 b;
    float padding2;
} Line;

typedef struct Point
{
    vec3 color;
    float padding;
    vec3 position;
    float pointSize;
} Point;

// Represents some z = f(x, y)
// The grid is n x n
// Instead of scale, allow the user to define a surface width dimension
// TODO: allow for rectangular domains instead of just square
typedef struct Surface
{
    float* vertices; // Vertex data (height, r, g, b). (x, y) -> vertices[x + y * n]
    ivec2 numPoints; // The number of points along the (x-axis, y-axis)
    vec2 dimensions; // The dimensions of the surface (width, height)
    vec3 origin; // The coordinates of the surface center in world space
    VertexArray vertexArray;
} Surface;

// Initializes polygons; must be called before other functions
void PolygonInitialize();

// Initializes a circle and returns its pointer
Circle* PolygonCircle(vec2 position, float radius, vec3 color);

// Allocates `count` contiguous circles and returns a pointer
Circle* PolygonCircles(unsigned int count);

// Initializes a rectangle and returns its pointer
Rect* PolygonRect(vec2 position, float width, float height, vec3 color);

// Allocates `count` contiguous rectangles and returns a pointer
Rect* PolygonRects(unsigned int count);

// Initializes a 2d line and returns its pointer
Line2D* PolygonLine2D(vec2 a, vec2 b, vec3 color);

// Allocates `count` contiguous 2d lines and returns a pointer
Line2D* PolygonLine2Ds(unsigned int count);

// Initializes a line and returns its pointer
Line* PolygonLine(vec3 a, vec3 b, vec3 color);

// Allocates `count` contiguous lines and returns a pointer
Line* PolygonLines(unsigned int count);

// Initializes a point and returns its pointer
Point* PolygonPoint(vec3 position, float pointSize, vec3 color);

// Allocates `count` contiguous points and returns a pointer
Point* PolygonPoints(unsigned int count);

void SurfaceInitialize(Surface* surface, float* data, vec2 dimensions, vec3 origin, ivec2 numPoints);

void SurfaceDraw(Surface* surface);

// Updates the view-perspective matrix used to transform polygons
void PolygonUpdateViewPerspectiveMatrix(mat4 vpMatrix);

// Renders polygons
void PolygonRenderPolygons();