#pragma once

#include <glm/glm.hpp>

namespace CameraDefaults {
	constexpr float NEAR_PLANE = 0.1f;
	constexpr float FAR_PLANE = 100.0f;
	constexpr float ZOOM = 45.0f;
	constexpr float YAW = -90.0f;
	constexpr float PITCH = 0.0f;
	constexpr float SPEED = 2.5f;
	constexpr float SENSITIVITY = 0.1f;
}


enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class Camera {
public:
	glm::vec3 GetPosition() const { return Position; }
	glm::vec3 GetFront() const { return Front; }

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = CameraDefaults::YAW, float pitch = CameraDefaults::PITCH)
		: Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up),
		  Yaw(yaw), Pitch(pitch), MovementSpeed(CameraDefaults::SPEED), MouseSensitivity(CameraDefaults::SENSITIVITY), Zoom(CameraDefaults::ZOOM) {
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix() const;

	virtual glm::mat4 GetProjectionMatrix(float aspectRatio) const = 0;

	virtual ~Camera() = default;

	virtual void ProcessKeyboard(CameraMovement direction, float deltaTime) = 0;
	virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
	virtual void ProcessMouseScroll(float yoffset) = 0;

protected:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	void UpdateCameraVectors();
};


