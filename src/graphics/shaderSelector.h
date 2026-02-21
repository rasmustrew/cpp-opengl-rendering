#pragma once

#include "graphics/shader.h"
#include <GLFW/glfw3.h>
#include <vector>

class ShaderSelector {
public:
	void Add(Shader shader);
	Shader& Active();
	void ProcessInput(GLFWwindow* window);
private:
	std::vector<Shader> Shaders;
	size_t ActiveIndex = 0;
};
