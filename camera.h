#pragma once
#include <glm/glm.hpp>

struct camera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float fov = 45.0f;
};

camera createCamera();
