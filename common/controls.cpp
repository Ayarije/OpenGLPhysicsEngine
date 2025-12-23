#include "controls.hpp"

namespace controls {
    GLFWwindow* window = NULL;
    int window_width = -1;
    int window_height = -1;
    double lastTime = -1;

    bool initialized = false;

    glm::vec3 position = glm::vec3( 3, -2, 3 );
    float horizontalAngle = 3.14f;
    float verticalAngle = 1.57f;
    float initialFoV = 45.0f;
    float currentFoV = initialFoV;

    float speed = 3.0f;
    float mouseSpeed = 0.05f;
    float zoomSpeed = 2.0f;
}

using namespace controls;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    currentFoV -= (float)yoffset * zoomSpeed;

    if (currentFoV < 1.0f) currentFoV = 1.0f;
    if (currentFoV > 120.0f) currentFoV = 120.0f;
}

int controls::initControls(GLFWwindow* window) {
    controls::window = window;
    glfwGetWindowSize(window, &controls::window_width, &controls::window_height);
    controls::lastTime = glfwGetTime();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (glfwSetScrollCallback) { 
        glfwSetScrollCallback(window, scroll_callback);
    }

    controls::initialized = true;
    return 1;
}

int controls::getDeltaTime(float* deltaTime) {
    if (!initialized) return 0;

    double now = glfwGetTime();
    *deltaTime = (float) (now - lastTime);
    lastTime = now;

    return 1;
}

int controls::computeMatricesfromInputs(glm::mat4* ProjectionMatrix, glm::mat4* ViewMatrix) {
    if (!initialized) return 0;

    float deltaTime;
    controls::getDeltaTime(&deltaTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    static double lastX = window_width / 2.0;
    static double lastY = window_height / 2.0;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = float(lastX - xpos);
    float yoffset = float(lastY - ypos);

    lastX = xpos;
    lastY = ypos;

    horizontalAngle += mouseSpeed * deltaTime * xoffset; 
    verticalAngle   += mouseSpeed * deltaTime * yoffset;

    glm::vec3 direction( 
        cos(verticalAngle) * sin(horizontalAngle), 
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle) 
    );

    glm::vec3 right = glm::vec3( 
        sin(horizontalAngle - 3.14f/2.0f), 
        0, 
        cos(horizontalAngle - 3.14f/2.0f) 
    );

    glm::vec3 up = glm::cross( right, direction );
    glm::vec3 globalUp = glm::vec3(0, -1, 0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += direction * deltaTime * speed; 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= direction * deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position += right * deltaTime * speed; 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position -= right * deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += globalUp * deltaTime * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) position -= globalUp * deltaTime * speed;

    *ProjectionMatrix = glm::perspective(glm::radians(currentFoV), 4.0f / 3.0f, 0.1f, 100.0f);
    *ViewMatrix = glm::lookAt(
        position,
        position + direction,
        up
    );

    return 1;
}