#pragma once
#include "util.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct light {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::mat4 model;
};



inline void updateLight(light& l) {
	glm::vec3 lightColor;
	lightColor.x = sin(getTime() * 2.0f);
	lightColor.y = sin(getTime() * 0.7f);
	lightColor.z = sin(getTime() * 1.3f);

	/*glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
	l.diffuse = diffuseColor;
	l.ambient = ambientColor;*/

	float lightX = 1.0f + sin(getTime()) * 10.0f;
	l.model[3][0] = lightX;


}

inline light getDefaultLight() {
	light l;
	glm::vec3 position = glm::vec3(-1.2f, 1.0f, 2.0f);

	l.model = glm::mat4(1.0f);
	l.model = glm::translate(l.model, position);
	l.model = glm::scale(l.model, glm::vec3(0.2f));
	l.ambient = glm::vec3(0.2f);
	l.diffuse = glm::vec3(0.5f);
	l.specular = glm::vec3(1.0f);
	return l;
}
