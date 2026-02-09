#pragma once
#include "camera.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct mvpMatrices {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

mvpMatrices createMvpMatrices(const Camera& cam) {
	mvpMatrices matrices;
	matrices.model = glm::mat4(1.0f);
	matrices.model = glm::mat4(1.0F);
	matrices.view = cam.GetViewMatrix();
	matrices.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
	return matrices;
}

float getTime() {
	return static_cast<float>(glfwGetTime());
}

void setupGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
}




