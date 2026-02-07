#include "camera.h"
#include "mouse.h"
#include "window.h"
#include <glm/gtc/matrix_transform.hpp>

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	windowCallbackData* data = static_cast<windowCallbackData*>(glfwGetWindowUserPointer(window));
	mouseState& state = data->mouse;
	camera& cam = data->cam;

	float xPos = static_cast<float>(xpos);
	float yPos = static_cast<float>(ypos);

	if (state.firstMouse) // initially set to true
	{
		state.lastX = xPos;
		state.lastY = yPos;
		state.firstMouse = false;
	}

	float xoffset = xPos - state.lastX;
	float yoffset = state.lastY - yPos; // reversed since y-coordinates range from bottom to top
	state.lastX = xPos;
	state.lastY = yPos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	state.yaw += xoffset;
	state.pitch += yoffset;
	if (state.pitch > 89.0f)
		state.pitch = 89.0f;
	if (state.pitch < -89.0f)
		state.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(state.yaw)) * cos(glm::radians(state.pitch));
	direction.y = sin(glm::radians(state.pitch));
	direction.z = sin(glm::radians(state.yaw)) * cos(glm::radians(state.pitch));
	cam.front = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double, double yoffset) {
	windowCallbackData* data = static_cast<windowCallbackData*>(glfwGetWindowUserPointer(window));
	camera& cam = data->cam;
	cam.fov -= (float)yoffset;
	if (cam.fov < 1.0f)
		cam.fov = 1.0f;
	if (cam.fov > 45.0f)
		cam.fov = 45.0f;
}
