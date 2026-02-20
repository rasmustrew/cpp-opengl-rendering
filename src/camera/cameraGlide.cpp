#include "cameraGlide.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 CameraGlide::GetProjectionMatrix(float aspectRatio) const {
	return glm::perspective(glm::radians(Zoom), aspectRatio, CameraDefaults::NEAR_PLANE, CameraDefaults::FAR_PLANE);
}

void CameraGlide::ProcessKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD) {
		Position += Front * velocity;
	}
	if (direction == CameraMovement::BACKWARD) {
		Position -= Front * velocity;
	}
	if (direction == CameraMovement::LEFT) {
		Position -= Right * velocity;
	}
	if (direction == CameraMovement::RIGHT) {
		Position += Right * velocity;
	}
}

void CameraGlide::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch) {
		if (Pitch > 89.0f) {
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f) {
			Pitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}

void CameraGlide::ProcessMouseScroll(float yoffset) {
	Zoom -= yoffset;
	if (Zoom < 1.0f) {
		Zoom = 1.0f;
	}
	if (Zoom > 45.0f) {
		Zoom = 45.0f;
	}
}
