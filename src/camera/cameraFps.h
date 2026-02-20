#pragma once

#include "camera.h"
#include <glm/glm.hpp>

class CameraFps : public Camera {
public:
	glm::mat4 GetProjectionMatrix(float aspectRatio) const override;
	void ProcessKeyboard(CameraMovement direction, float deltaTime) override;
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
	void ProcessMouseScroll(float yoffset) override;
};


