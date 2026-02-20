#pragma once
#include <glm/glm.hpp>


struct SpotLight {
	glm::vec3 Ambient = glm::vec3(0.1f);
	glm::vec3 Diffuse = glm::vec3(0.8f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float CutOff = glm::cos(glm::radians(12.5f));
	float OuterCutOff = glm::cos(glm::radians(15.0f));
};

struct DirectionalLight {
	glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 Ambient = glm::vec3(0.1f);
	glm::vec3 Diffuse = glm::vec3(0.2f);
	glm::vec3 Specular = glm::vec3(0.3f);
};

struct PointLight {
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Ambient = glm::vec3(0.2f);
	glm::vec3 Diffuse = glm::vec3(0.8f);
	glm::vec3 Specular = glm::vec3(1.0f);
	float Constant = 1.0f;
	float Linear = 0.35f;
	float Quadratic = 0.44f;
};
