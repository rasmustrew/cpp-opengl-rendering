#pragma once
#include "camera.h"
#include "mouse.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct windowCallbackData {
	camera& cam;
	mouseState& mouse;
};

auto setupWindow() -> GLFWwindow*;
void setupInput(GLFWwindow* window, windowCallbackData& callbackData);
