#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

inline float GetTime() {
	return static_cast<float>(glfwGetTime());
}

inline void SetupGlad() {
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
}




