#pragma once
#include "shader.h"
#include "util.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


struct spotLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float cutOff;
	float outerCutOff;
};

struct directionalLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
};

struct pointLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::mat4 model;

	float constant = 1.0f;
	float linear = 0.35f;
	float quadratic = 0.44f;
};

inline glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

inline void setSpotLightUniforms(Shader& shader, spotLight& light, Camera& cam) {
	shader.setVec3("spotLight.ambient", light.ambient);
	shader.setVec3("spotLight.diffuse", light.diffuse);
	shader.setVec3("spotLight.specular", light.specular);
	shader.setFloat("spotLight.cutOff", light.cutOff);
	shader.setFloat("spotLight.outerCutOff", light.outerCutOff);
	shader.setVec3("spotLight.position", cam.Position);
	shader.setVec3("spotLight.direction", cam.Front);
}

inline void setDirectionalLightUniforms(Shader& shader, directionalLight& light) {
	shader.setVec3("dirLight.direction", light.direction);
	shader.setVec3("dirLight.ambient", light.ambient);
	shader.setVec3("dirLight.diffuse", light.diffuse);
	shader.setVec3("dirLight.specular", light.specular);
}

inline void setPointLightUniforms(Shader& shader, pointLight& light, size_t index) {
	std::string prefix = "pointLights[" + std::to_string(index) + "]";
	shader.setVec3(prefix + ".position", glm::vec3(light.model[3]));
	shader.setVec3(prefix + ".ambient", light.ambient);
	shader.setVec3(prefix + ".diffuse", light.diffuse);
	shader.setVec3(prefix + ".specular", light.specular);
	shader.setFloat(prefix + ".constant", light.constant);
	shader.setFloat(prefix + ".linear", light.linear);
	shader.setFloat(prefix + ".quadratic", light.quadratic);
}

inline spotLight getDefaultSpotLight() {
	spotLight l;
	l.ambient = glm::vec3(0.1f);
	l.diffuse = glm::vec3(0.8f);
	l.specular = glm::vec3(1.0f);
	l.cutOff = glm::cos(glm::radians(12.5f));
	l.outerCutOff = glm::cos(glm::radians(15.0f));
	return l;
}


inline directionalLight getDefaultDirectionalLight() {
	directionalLight l;
	l.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	l.ambient = glm::vec3(0.1f);
	l.diffuse = glm::vec3(0.2f);
	l.specular = glm::vec3(0.3f);
	return l;
}

inline pointLight getDefaultPointLight() {
	pointLight l;
	l.model = glm::mat4(1.0f);
	l.ambient = glm::vec3(0.2f);
	l.diffuse = glm::vec3(0.8f);
	l.specular = glm::vec3(1.0f);
	return l;
}
