#pragma once

#include <glm/glm.hpp>

struct material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct object3D {
	glm::mat4 model;
	material mat;
};