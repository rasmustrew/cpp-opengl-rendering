#include "camera.h"
#include "input.h"
#include "window.h"
#include <glm/gtc/matrix_transform.hpp>


void setupInput(GLFWwindow* window, windowCallbackData& callbackData) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowUserPointer(window, &callbackData);
	glfwSetScrollCallback(window, scroll_callback);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	windowCallbackData* data = static_cast<windowCallbackData*>(glfwGetWindowUserPointer(window));
	mouseState& state = data->mouse;
	Camera& cam = data->cam;

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

	cam.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double, double yoffset) {
	windowCallbackData* data = static_cast<windowCallbackData*>(glfwGetWindowUserPointer(window));
	Camera& cam = data->cam;
	cam.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processKeyboardInput(GLFWwindow* window, Camera& cam, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.ProcessKeyboard(RIGHT, deltaTime);
}
