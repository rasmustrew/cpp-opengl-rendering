#include "shaderSelector.h"
#include <cassert>

void ShaderSelector::Add(Shader shader) {
	Shaders.push_back(std::move(shader));
}

Shader& ShaderSelector::Active() {
	assert(!Shaders.empty());
	return Shaders[ActiveIndex];
}

void ShaderSelector::ProcessInput(GLFWwindow* window) {
	for (size_t i = 0; i < Shaders.size() && i < 9; i++) {
		if (glfwGetKey(window, GLFW_KEY_1 + static_cast<int>(i)) == GLFW_PRESS) {
			ActiveIndex = i;
			break;
		}
	}
}
