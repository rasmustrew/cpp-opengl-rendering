#pragma once
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct mouseState {
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
};

struct windowCallbackData {
	Camera& cam;
	mouseState& mouse;
};

void setupInput(GLFWwindow* window, windowCallbackData& callbackData);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyboardInput(GLFWwindow* window, Camera& cam, float deltaTime);

