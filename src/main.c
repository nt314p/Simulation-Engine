#include <cglm\cglm.h>
#include <cglm\version.h>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include "debug.h"
#include "renderer.h"
#include "shader.h"
#include "camera.h"
#include "polygon.h"
#include "input.h"
#include "color.h"
#include "physics.h"

static const int WIDTH = 1280;
static const int HEIGHT = 720;

static void ProcessInput(GLFWwindow* window, float deltaTime);

static float zoom = 12.0f;
static int paused = 0;

// TODO: is this uniformly distributed?
static inline float RandomRange(float min, float max)
{
    return rand() * (max - min) / RAND_MAX + min;
}

GLFWwindow* SimInitWindow(int width, int height, const char* title, int isFullscreen);

GLFWwindow* window;

double startTime;
double lastFPSUpdate;
double currentSimTime;

Line2D* bounds;
Surface s;

GLFWwindow* Initialize()
{
    window = SimInitWindow(WIDTH, HEIGHT, "Viewer", false);
    if (window == NULL)
    {
        printf("Error!\n"); // TODO: handle this better
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("cglm version: %d.%d.%d\n",
        CGLM_VERSION_MAJOR, CGLM_VERSION_MINOR, CGLM_VERSION_PATCH);

    int maxUBOSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
    printf("Max uniform buffer size: %d K\n", maxUBOSize / 1024);

    int maxSSBOSize;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOSize);
    printf("Max shader storage buffer size: %d K\n", maxSSBOSize / 1024);

    CameraUsePerspective(45.0f, ((float)WIDTH) / HEIGHT, 0.1f, 400.0f);
    //CameraUseOrthographic(((float)WIDTH) / HEIGHT, 10.0f);

    vec3 origin = { 0, 0, 0 };
    PolygonLine(origin, (vec3) { 1, 0, 0 }, COLOR_RED);
    PolygonLine(origin, (vec3) { 0, 1, 0 }, COLOR_GREEN);
    PolygonLine(origin, (vec3) { 0, 0, 1 }, COLOR_BLUE);

    PolygonPoint((vec3) { 0, 1, 0 }, 2.0f, COLOR_BEIGE);

    int nX = 256;
    int nY = 256;
    double sizeX = 20;
    double sizeY = 20;

    float* surfaceData = malloc(nX * nY * 4 * sizeof(float));
    for (int c = 0; c < nY; c++)
    {
        for (int r = 0; r < nX; r++)
        {
            double x = sizeX * (r / (nX - 1.0)) - sizeX / 2.0;
            double y = sizeY * (c / (nY - 1.0)) - sizeY / 2.0;

            float height = sin(0.5 * (x * x + y * y));// * (sin(x + y) + cos(x - y));//10 * x * y / (expf(x * x + y * y));
            int vertIndex = r + nX * c;
            surfaceData[vertIndex * 4 + 0] = height;
            glm_vec3_copy(COLOR_FLORALWHITE, &surfaceData[vertIndex * 4 + 1]);
        }
    }

    SurfaceInitialize(&s, surfaceData, (vec2) { sizeX, sizeY }, (vec3) { 0, 0, 0 }, (ivec2) { nX, nY });

    startTime = glfwGetTime();
    lastFPSUpdate = startTime;

    //GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    GLCall(glClearColor(0.1f, 0.15f, 0.2f, 1.0f));

    return window;
}

float camYaw = 0;
float camPitch = 0;

void Update(float deltaTime)
{
    double currentFrameTime = glfwGetTime();
    currentSimTime = currentFrameTime - startTime;

    if (lastFPSUpdate + 0.5 < currentFrameTime)
    {
        char fpsText[30];
        snprintf(fpsText, 30, "Viewer | Render: %3.2f ms", deltaTime * 1000.0);

        glfwSetWindowTitle(window, fpsText);
        lastFPSUpdate += 0.5;
    }

    ProcessInput(window, deltaTime);

    vec2 scrollDelta;
    InputScrollDelta(scrollDelta);

    zoom += scrollDelta[1];
    zoom = fmax(zoom, 0.1f);
    //CameraOrthographicZoom(zoom);

    vec2 mouseDelta;
    InputMouseDelta(mouseDelta);
    camYaw += 0.3f * mouseDelta[0];
    camPitch -= 0.3f * mouseDelta[1];
    camPitch = glm_clamp(camPitch, -90.0f, 90.0f);

    CameraRotate(camYaw, camPitch, 0);

    vec2 mouseCoords;
    InputMouseCoords(mouseCoords);

    vec3 worldMouseCoords;
    CameraViewToWorldPoint(mouseCoords, worldMouseCoords);
}

void Render(float deltaTime)
{
    //SurfaceDraw(&s);
}

static void ProcessInput(GLFWwindow* window, float deltaTime)
{
    float speed = zoom / 2.0f;
    vec3 movement = { 0.0f };

    InputGetAxes3D(window, movement); // TODO: passing vec3 as a vec2?

    if (InputKeyPressed(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, 1);
    }

    if (InputKeyPressed(window, GLFW_KEY_SPACE))
    {
        paused = !paused;
    }

    glm_vec3_scale(movement, deltaTime * speed, movement);

    CameraTranslateRelative(movement);
}