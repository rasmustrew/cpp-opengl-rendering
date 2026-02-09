#pragma once
#include "camera.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

float getTime() {
	return static_cast<float>(glfwGetTime());
}

void setupGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
}




