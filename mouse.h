#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct mouseState {
	float lastX = 0;
	float lastY = 0;
	float yaw = 0;
	float pitch = 0;
	bool firstMouse = true;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
