#include <GLFW\glfw3.h>
#include <cglm\cglm.h>
#include "input.h"
#include "debug.h"


static vec2 MouseCoords; // Screen space coordinates (+x axis = right; +y axis = down)
static vec2 MouseViewCoords; // In normalized upright axis [-1, 1]
static vec2 MouseDelta; // In pixels per frame
static vec2 ScrollDelta;

static int WindowWidth;
static int WindowHeight;

static void ScrollCallback(GLFWwindow* window, double deltaX, double deltaY)
{
    ScrollDelta[0] = deltaX;
    ScrollDelta[1] = deltaY;
}

static void MouseCallback(GLFWwindow* window, double x, double y)
{
    static double prevX, prevY;
    static bool firstCall = true;
    static double originX = 0;
    static double originY = 0;
    /*
    Since using GLFW_CURSOR_DISABLED causes infinite movement, we use an offset
    origin to define a rectanglular screen. Whenever the cursor moves outside
    this rectangle, we move the origin so the rectangle always contains the origin.
    The screenspace coordinates are then defined by the mouse's position within
    this rectangle.

    We continue to use the raw coordinates to compute the mouse delta.
    */

    if (x < originX) originX = x;
    if (y < originY) originY = y;
    if (x > originX + WindowWidth - 1) originX = x - (WindowWidth - 1);
    if (y > originY + WindowHeight - 1) originY = y - (WindowHeight - 1);

    float xReal = x - originX;
    float yReal = y - originY;

    float xView = xReal / (WindowWidth - 1);
    float yView = yReal / -(WindowHeight - 1); // Assumes height is smaller than width

    xView = 2 * xView - 1;
    yView = 2 * yView + 1;

    if (firstCall)
    {
        prevX = x;
        prevY = y;
        firstCall = false;
    }

    float dx = x - prevX;
    float dy = -(y - prevY);

    prevX = x;
    prevY = y;

    MouseViewCoords[0] = xView;
    MouseViewCoords[1] = yView;
    MouseCoords[0] = xReal;
    MouseCoords[1] = yReal;
    MouseDelta[0] = dx;
    MouseDelta[1] = dy;
}

void InputInitialize(GLFWwindow* window)
{
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    // TODO: this assumes that window size doesn't change after init
    glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
}

void InputMouseDelta(vec2 mouseDelta)
{
    glm_vec2_copy(MouseDelta, mouseDelta);
}

// Returns the mouse coordinates in screen space [0, W - 1] x [0, H - 1]
void InputMouseCoords(vec2 mouseCoords)
{
    glm_vec2_copy(MouseCoords, mouseCoords);
}

// Returns the mouse coordinates in view space [-1, 1] x [-1, 1]
void InputMouseViewCoords(vec2 mouseViewCoords)
{
    glm_vec2_copy(MouseCoords, mouseViewCoords);
}

void InputScrollDelta(vec2 scrollDelta)
{
    glm_vec2_copy(ScrollDelta, scrollDelta);
}

// Gets a 2d vector representing axis input
// x [-1, 1]: horizontal input
// y [-1, 1]: vertical input
void InputGetAxes2D(GLFWwindow* window, vec2 axisInput)
{
    // TODO: store window ref in this file?
    // TODO: custom horizontal and vertical axis keybinds
    axisInput[0] = InputKeyPressed(window, GLFW_KEY_D) - InputKeyPressed(window, GLFW_KEY_A);
    axisInput[1] = InputKeyPressed(window, GLFW_KEY_W) - InputKeyPressed(window, GLFW_KEY_S);
}

void InputGetAxes3D(GLFWwindow* window, vec3 axisInput)
{
    // TODO: store window ref in this file?
    // TODO: custom horizontal and vertical axis keybinds
    axisInput[0] = InputKeyPressed(window, GLFW_KEY_D) - InputKeyPressed(window, GLFW_KEY_A);
    axisInput[1] = InputKeyPressed(window, GLFW_KEY_LEFT_SHIFT) - InputKeyPressed(window, GLFW_KEY_LEFT_CONTROL);
    axisInput[2] = InputKeyPressed(window, GLFW_KEY_W) - InputKeyPressed(window, GLFW_KEY_S);
}

// Resets all input values. Should be called at the end of every frame
void InputReset()
{
    glm_vec2_zero(MouseDelta);
    glm_vec2_zero(ScrollDelta);
}

int InputKeyPressed(GLFWwindow* window, int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

int InputKeyDown(GLFWwindow* window, int key)
{
    return glfwGetKey(window, key) == GLFW_KEY_DOWN;
}