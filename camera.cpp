#include "camera.h"

camera createCamera() {
	camera cam;
	cam.position = glm::vec3(0.0f, 0.0f, 3.0f);
	cam.front = glm::vec3(0.0f, 0.0f, -1.0f);
	cam.up = glm::vec3(0.0f, 1.0f, 0.0f);
	return cam;
}
