#include <string.h>
#include <cglm\cglm.h>
#include "polygon.h"
#include "shader.h"
#include "renderer.h"
#include "debug.h"

// The maximum depth used for the circle generation algorithm
// A higher level yields a better circle approximation
// See https://www.humus.name/index.php?page=News&ID=228
#define MaxCircleGenLevel 4 
#define CircleVertexCount (3 * (1 << MaxCircleGenLevel))
#define CircleTriangles (3 * ((1 << MaxCircleGenLevel) - 1) + 1)
static const int VerticesPerLine = 2;
static const int VerticesPerRect = 4;

static const char* BasicFragShaderPath = "shaders/BasicFrag.frag";
static const char* CircleVertShaderPath = "shaders/InstancedCircle.vert";
static const char* RectVertShaderPath = "shaders/InstancedRect.vert";
static const char* LineVertShaderPath = "shaders/InstancedLine.vert";
static const char* Line2DVertShaderPath = "shaders/InstancedLine2D.vert";
static const char* PointVertShaderPath = "shaders/InstancedPoint.vert";
static const char* SurfaceVertShaderPath = "shaders/Surface.vert";

static InstancedObject instancedCircle;
static InstancedObject instancedRect;
static InstancedObject instancedLine;
static InstancedObject instancedLine2D;
static InstancedObject instancedPoint;

static bool IsInitialized = false;

// TODO: make primitive instance struct

static const int MaxCircleCount = 2048;
static const int MaxRectCount = 2048;
static const int MaxLineCount = 1024;
static const int MaxLine2DCount = 2048;
static const int MaxPointCount = 2048;

static mat4 vpMatrix;
static UniformBuffer vpMatrixUB;

static unsigned int surfaceShaderId;

// Initializes vertices forming a tessellated unit circle
// `vertData` should hold `3 * CircleTriangles` `vec2`s
static void GenerateUnitCircleVertices(float* vertData)
{
    vec2 vertLookup[CircleVertexCount];

    for (int i = 0; i < CircleVertexCount; i++)
    {
        float angle = (2 * M_PI / CircleVertexCount) * i;

        vertLookup[i][0] = cosf(angle);
        vertLookup[i][1] = sinf(angle);
    }

    vec2* vertDataVec2 = (vec2*)vertData;

    int globalIndex = 0;

    // render n = 4 levels
    // vertex count = 3 * 2^n = 3 * 2^4 = 48
    for (int n = 0; n <= MaxCircleGenLevel; n++) {
        int triangleCount = n == 0 ? 1 : 3 * (1 << (n - 1));
        int stride = 1 << (MaxCircleGenLevel - n);

        for (int i = 0; i < triangleCount; i++)
        {
            int vIndex = i * stride * 2; // base vertex index in lookup

            glm_vec2_copy(vertLookup[vIndex], vertDataVec2[globalIndex]);
            glm_vec2_copy(vertLookup[vIndex + stride], vertDataVec2[globalIndex + 1]);
            glm_vec2_copy(vertLookup[(vIndex + 2 * stride) % CircleVertexCount], vertDataVec2[globalIndex + 2]);

            globalIndex += 3;
        }
    }    
}

// Initializes an instanced object. Be sure to also call `InstancedObjectInializeVertices`
static void InstancedObjectInitialize(InstancedObject* object, const char* vertShaderPath, unsigned int maxCount, unsigned int objectSize, UniformBuffer* vpMatrix)
{
    unsigned int shaderId = ShaderCreate(vertShaderPath, BasicFragShaderPath);
    object->shaderId = shaderId;
    object->MaxObjectCount = maxCount;
    object->size = objectSize;

    unsigned int bufferSize = maxCount * objectSize;
    object->objects = calloc(maxCount, objectSize);

    UniformBufferInitialize(&object->instanceDataBuffer, object->objects, bufferSize, GL_DYNAMIC_DRAW);
    ShaderBindUniformBuffer(shaderId, "Objects", &object->instanceDataBuffer);
    ShaderBindUniformBuffer(shaderId, "Matrices", vpMatrix);
}

// This technically can be combined with the previous call, but it was getting too long!
// If `vertices` is null, this indicates the instanced object requires no vertex data
// to render. The `floatsPerVertex` parameter is ignored but `verticesPerObject` parameter is
// still required.
static void InstancedObjectInitializeVertices(InstancedObject* object, float* vertices, int verticesPerObject, int floatsPerVertex, GLenum mode)
{
    object->mode = mode;
    object->verticesPerObject = verticesPerObject;

    VertexArrayInitialize(&object->vao);

    if (vertices)
    {
        VertexArrayBind(&object->vao);
        VertexBufferInitialize(&object->vao, vertices, verticesPerObject * floatsPerVertex * sizeof(float), GL_STATIC_DRAW);
        VertexAttribPointerFloats(0, floatsPerVertex, floatsPerVertex * sizeof(float));
    }

    VertexArrayUnbind();
}

void PolygonInitialize()
{
    UniformBufferInitialize(&vpMatrixUB, vpMatrix, sizeof(mat4), GL_DYNAMIC_DRAW);

    float* circleVertices = malloc(sizeof(vec2) * CircleTriangles * 3);
    GenerateUnitCircleVertices(circleVertices);
    InstancedObjectInitialize(&instancedCircle, CircleVertShaderPath, MaxCircleCount, sizeof(Circle), &vpMatrixUB);
    InstancedObjectInitializeVertices(&instancedCircle, circleVertices, 3 * CircleTriangles, 2, GL_TRIANGLES);
    free(circleVertices);

    // Unit square of side length 1 centered at the origin
    float unitSquareVertices[8] = { 0.5f, 0.5f , -0.5f, 0.5f , -0.5f, -0.5f , 0.5f, -0.5f };
    InstancedObjectInitialize(&instancedRect, RectVertShaderPath, MaxRectCount, sizeof(Rect), &vpMatrixUB);
    InstancedObjectInitializeVertices(&instancedRect, unitSquareVertices, VerticesPerRect, 2, GL_TRIANGLE_FAN);

    InstancedObjectInitialize(&instancedLine, LineVertShaderPath, MaxLineCount, sizeof(Line), &vpMatrixUB);
    InstancedObjectInitializeVertices(&instancedLine, NULL, VerticesPerLine, 0, GL_LINES);

    InstancedObjectInitialize(&instancedLine2D, Line2DVertShaderPath, MaxLine2DCount, sizeof(Line2D), &vpMatrixUB);
    InstancedObjectInitializeVertices(&instancedLine2D, NULL, VerticesPerLine, 0, GL_LINES);

    InstancedObjectInitialize(&instancedPoint, PointVertShaderPath, MaxPointCount, sizeof(Point), &vpMatrixUB);
    InstancedObjectInitializeVertices(&instancedPoint, NULL, 1, 0, GL_POINTS);

    surfaceShaderId = ShaderCreate(SurfaceVertShaderPath, BasicFragShaderPath);
    ShaderBindUniformBuffer(surfaceShaderId, "Matrices", &vpMatrixUB);

    GLCall(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));

    IsInitialized = true;
}

// Returns `count` contiguous instances of the passed `InstancedObject`
static void* InstancedObjectAllocate(InstancedObject* object, unsigned int count)
{
    void* objs = ((uint8_t*)object->objects) + object->count * object->size;
    object->count += count;
    return objs;
}

// Draws all instanced objects if there are any
static void InstancedObjectDraw(InstancedObject* object)
{
    if (object->count == 0) return;
    UniformBufferUpdateRange(&object->instanceDataBuffer, 0, object->size * object->count);
    ShaderUse(object->shaderId);
    VertexArrayBind(&object->vao);
    GLCall(glDrawArraysInstanced(object->mode, 0, object->verticesPerObject, object->count));
}

Circle* PolygonCircle(vec2 position, float radius, vec3 color)
{
    Circle* c = InstancedObjectAllocate(&instancedCircle, 1);
    glm_vec2_copy(position, c->position);
    c->radius = radius;
    glm_vec3_copy(color, c->color);

    return c;
}

Circle* PolygonCircles(unsigned int count)
{
    return InstancedObjectAllocate(&instancedCircle, count);
}

Rect* PolygonRect(vec2 position, float width, float height, vec3 color)
{
    Rect* r = InstancedObjectAllocate(&instancedRect, 1);
    glm_vec2_copy(position, r->position);
    r->width = width;
    r->height = height;
    glm_vec3_copy(color, r->color);

    return r;
}

Rect* PolygonRects(unsigned int count)
{
    return InstancedObjectAllocate(&instancedRect, count);
}

Line2D* PolygonLine2D(vec2 a, vec2 b, vec3 color)
{
    Line2D* l = InstancedObjectAllocate(&instancedLine2D, 1);
    glm_vec2_copy(a, l->a);
    glm_vec2_copy(b, l->b);
    glm_vec3_copy(color, l->color);

    return l;
}

Line2D* PolygonLine2Ds(unsigned int count)
{
    return InstancedObjectAllocate(&instancedLine2D, count);
}

Line* PolygonLine(vec3 a, vec3 b, vec3 color)
{
    Line* l = InstancedObjectAllocate(&instancedLine, 1);
    glm_vec3_copy(a, l->a);
    glm_vec3_copy(b, l->b);
    glm_vec3_copy(color, l->color);

    return l;
}

Line* PolygonLines(unsigned int count)
{
    return InstancedObjectAllocate(&instancedLine, count);
}

Point* PolygonPoint(vec3 position, float pointSize, vec3 color)
{
    Point* p = InstancedObjectAllocate(&instancedPoint, 1);
    glm_vec3_copy(position, p->position);
    p->pointSize = pointSize;
    glm_vec3_copy(color, p->color);

    return p;
}

Point* PolygonPoints(unsigned int count)
{
    return InstancedObjectAllocate(&instancedPoint, count);
}

// TODO: improve tessellation: https://skytiger.wordpress.com/2010/11/28/xna-large-terrain/
void SurfaceInitialize(Surface* surface, float* data, vec2 dimensions, vec3 origin, ivec2 numPoints)
{
    glm_vec2_copy(dimensions, surface->dimensions);
    glm_vec3_copy(origin, surface->origin);

    int nX = numPoints[0];
    int nY = numPoints[1];

    surface->numPoints[0] = nX;
    surface->numPoints[1] = nY;

    int dataSize = sizeof(vec4) * nX * nY;
    surface->vertices = data;

    VertexArray* va = &surface->vertexArray;

    VertexArrayInitialize(va);
    VertexArrayBind(va);

    VertexBufferInitialize(va, data, dataSize, GL_DYNAMIC_DRAW);
    VertexAttribPointerFloats(0, 1, 16); // height
    VertexAttribPointerFloats(1, 3, 16); // color rgb

    // An nX x nY grid contains (nX - 1) x (nY - 1) squares.
    // So we need 2 * (nX - 1) * (nY - 1) triangles,
    // which have 3 * 2 * (nX - 1) * (nY - 1) vertices.
    int numVertices = 3 * 2 * (nX - 1) * (nY - 1);
    unsigned int* indexData = malloc(sizeof(unsigned int) * numVertices);

    /* Triangulation
    (x,y)
     a---b
     |  /|
     | / |
     |/  |
     c---d (x + 1, y + 1)
    */

    int vertIndex = 0;
    for (int x = 0; x < nX - 1; x++)
    {
        for (int y = 0; y < nY - 1; y++)
        {
            int a = x + y * nX;
            int b = a + 1;
            int c = a + nX;
            int d = c + 1;
            indexData[vertIndex + 0] = a; // top left tri
            indexData[vertIndex + 1] = b;
            if ((x + y) % 2 == 0) // Alternative triangulation, removes artifacts??
            {
                indexData[vertIndex + 2] = c;
            }
            else
            {
                indexData[vertIndex + 2] = d;
            }
            //indexData[vertIndex + 2] = c;

            indexData[vertIndex + 3] = d; // bottom right tri
            indexData[vertIndex + 4] = c;
            //indexData[vertIndex + 5] = a;
            if ((x + y) % 2 == 0)
            {
                indexData[vertIndex + 5] = b;
            }
            else
            {
                indexData[vertIndex + 5] = a;
            }
            vertIndex += 6;
        }
    }

    IndexBufferInitialize(va, indexData, numVertices, GL_STATIC_DRAW);

    free(indexData); // TODO: hopefully we don't access this data again

    VertexArrayUnbind();
}

void SurfaceDraw(Surface* surface)
{
    VertexArray* va = &surface->vertexArray;
    VertexBufferUpdate(va);
    ShaderUse(surfaceShaderId);

    GLCall(glUniform3f(glGetUniformLocation(surfaceShaderId, "origin"), surface->origin[0], surface->origin[1], surface->origin[2]));
    GLCall(glUniform2f(glGetUniformLocation(surfaceShaderId, "dimensions"), surface->dimensions[0], surface->dimensions[1]));
    GLCall(glUniform2i(glGetUniformLocation(surfaceShaderId, "numPoints"), surface->numPoints[0], surface->numPoints[1]));

    VertexArrayBind(va);
    GLCall(glDrawElements(GL_TRIANGLES, va->indexBufferCount, GL_UNSIGNED_INT, 0))
}

void PolygonUpdateViewPerspectiveMatrix(mat4 m)
{
    glm_mat4_copy(m, vpMatrix);
    UniformBufferUpdate(&vpMatrixUB);
}

void PolygonRenderPolygons()
{
    InstancedObjectDraw(&instancedCircle);
    InstancedObjectDraw(&instancedRect);
    InstancedObjectDraw(&instancedLine);
    InstancedObjectDraw(&instancedLine2D);
    InstancedObjectDraw(&instancedPoint);
}