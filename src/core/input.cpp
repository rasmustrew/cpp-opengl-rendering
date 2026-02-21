#include "camera/camera.h"
#include "core/input.h"

#include <GLFW/glfw3.h>
#include <graphics/shaderSelector.h>

void SetupInput(GLFWwindow* window, WindowCallbackData& callbackData) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetWindowUserPointer(window, &callbackData);
	glfwSetScrollCallback(window, ScrollCallback);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	WindowCallbackData* data = static_cast<WindowCallbackData*>(glfwGetWindowUserPointer(window));
	MouseState& state = data->Mouse;
	Camera& cam = data->Cam;

	float xPos = static_cast<float>(xpos);
	float yPos = static_cast<float>(ypos);

	if (state.FirstMouse) // initially set to true
	{
		state.LastX = xPos;
		state.LastY = yPos;
		state.FirstMouse = false;
	}

	float xoffset = xPos - state.LastX;
	float yoffset = state.LastY - yPos; // reversed since y-coordinates range from bottom to top
	state.LastX = xPos;
	state.LastY = yPos;

	cam.ProcessMouseMovement(xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double, double yoffset) {
	WindowCallbackData* data = static_cast<WindowCallbackData*>(glfwGetWindowUserPointer(window));
	Camera& cam = data->Cam;
	cam.ProcessMouseScroll(static_cast<float>(yoffset));
}

void ProcessKeyboardInput(GLFWwindow* window, Camera& cam, ShaderSelector& shaderSelector, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}

	shaderSelector.ProcessInput(window);
}
