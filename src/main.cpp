// Current Function: Testing

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "core/terrain/terrain.h"
#include "core/camera/camera.h"
#include "core/render/render-modes.h"
#include "tools/raise_lower_brush/raise_lower_brush.h"

// Global Camera Settings
Camera camera(glm::vec3(0.0f, 2.0f, 5.0f));
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
bool firstMouse = true;

// Render Mode & Brush Instance
TerrainRenderMode renderMode;
Core::Tools::RaiseLowerBrush raiseLowerBrush;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Window Dimensions
int windowWidth = 800;
int windowHeight = 600;

// Callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Terrain& terrain);

// Raycast helper to project mouse cursor onto ground plane
bool raycastToTerrainPlane(Camera& cam, float screenX, float screenY, int width, int height, glm::vec3& outHitPoint) {
    float x = (2.0f * screenX) / width - 1.0f;
    float y = 1.0f - (2.0f * screenY) / height;

    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), aspectRatio, 0.1f, 100.0f);
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // Get camera position using public member or fallback method
#if defined(__cpp_member_variables) || true
    glm::vec3 camPos = cam.CameraPos;
#endif

    glm::vec3 rayDir = glm::normalize(glm::vec3(glm::inverse(cam.GetViewMatrix()) * rayEye));
    glm::vec3 rayOrigin = camPos;

    if (std::abs(rayDir.y) < 0.0001f) return false;

    float t = -rayOrigin.y / rayDir.y;
    if (t < 0.0f) return false;

    outHitPoint = rayOrigin + rayDir * t;
    return true;
}

// Main Function
int main()
{
    // GLFW Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Fullscreen Setup
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // Window Object
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Ariadnis", primaryMonitor, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    windowWidth = mode->width;
    windowHeight = mode->height;
    lastX = mode->width / 2.0f;
    lastY = mode->height / 2.0f;

    // Register Callbacks & Lock Mouse to Window
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // GLAD Initialize
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Viewport
    glViewport(0, 0, mode->width, mode->height);

    // Vertex Shader using View and Projection Matrices
    const char* vertexShaderSource = "#version 330 core \n"
        "layout (location = 0) in vec3 aPos; \n"
        "uniform mat4 view; \n"
        "uniform mat4 projection; \n"
        "void main() \n"
        "{ \n"
        " gl_Position = projection * view * vec4(aPos, 1.0); \n"
        "} \0";

    // Create shader object and shader source code
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER / VERTEX / COMPILATION_FAILED \n" << infoLog << std::endl;
    }

    // Fragment Shader
    const char* fragmentShaderSource = "#version 330 core \n"
        "out vec4 FragColor; \n"
        "uniform vec4 color; \n"
        "void main() \n"
        "{ \n"
        " FragColor = color; \n"
        "} \0";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER / FRAGMENT / COMPILATION_FAILED \n" << infoLog << std::endl;
    }

    // Link Shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER / PROGRAM / LINKING_FAILED \n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int colorLoc = glGetUniformLocation(shaderProgram, "color");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Instantiate and setup Terrain (Width, Depth, CellSize) -----------------------------------
    Terrain myTerrain(64, 64, 0.1f);
    myTerrain.generateMesh();
    myTerrain.setupBuffers();

    float aspectRatio = (float)mode->width / (float)mode->height;

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Delta time calculation
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input processing
        processInput(window, myTerrain);

        glClearColor(0.10f, 0.11f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Calculate 3D transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        // Draw Terrain using active Render Mode (SOLID / WIREFRAME / SOLID_WITH_WIREFRAME)
        renderMode.render(myTerrain, colorLoc);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up shader program
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// Input Function
void processInput(GLFWwindow* window, Terrain& terrain)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::DOWN, deltaTime);

    // Handle Render Mode Toggles (Key 'O')
    renderMode.handleInput(window);

    // Toggle Raise/Lower with Left Shift
    raiseLowerBrush.isLowering = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

    // Sculpt on Left Mouse Click
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec3 hitPoint;
        if (raycastToTerrainPlane(camera, static_cast<float>(mouseX), static_cast<float>(mouseY), windowWidth, windowHeight, hitPoint))
        {
            raiseLowerBrush.apply(terrain, hitPoint, deltaTime);
        }
    }
}

// Mouse movement callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // Only rotate camera if RIGHT mouse button is held down
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.processMouseMovement(xoffset, yoffset);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

// Mouse scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}