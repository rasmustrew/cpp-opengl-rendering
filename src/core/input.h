#pragma once

#include "camera/camera.h"
#include "graphics/shaderSelector.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct MouseState {
	float LastX = 0;
	float LastY = 0;
	bool FirstMouse = true;
};

struct WindowCallbackData {
	Camera& Cam;
	MouseState& Mouse;
};

void SetupInput(GLFWwindow* window, WindowCallbackData& callbackData);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessKeyboardInput(GLFWwindow* window, Camera& cam, ShaderSelector& shaderSelector, float deltaTime);

