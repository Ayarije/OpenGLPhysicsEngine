#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <cstdio>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace controls
{
    extern GLFWwindow* window;
    extern int window_width;
    extern int window_height;
    extern double lastTime;

    extern bool initialized;

    extern glm::vec3 position;
    extern float horizontalAngle;
    extern float verticalAngle;
    extern float initialFoV;
    extern float currentFoV;

    extern float speed;
    extern float mouseSpeed;
    extern float zoomSpeed;
    
    int initControls(GLFWwindow* window);
    int getDeltaTime(float* deltaTime);
    int computeMatricesfromInputs(glm::mat4* ProjectionMatrix, glm::mat4* ViewMatrix);
} // namespace controls

#endif
