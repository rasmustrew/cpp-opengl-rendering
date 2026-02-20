#include "camera/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {

glm::mat4 LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
	glm::vec3 direction = glm::normalize(position - target);
	glm::vec3 right = glm::normalize(glm::cross(up, direction));

	glm::vec4 right4 = glm::vec4(right, 0.0f);
	glm::vec4 up4 = glm::vec4(up, 0.0f);
	glm::vec4 direction4 = glm::vec4(direction, 0.0f);
	glm::vec4 base = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 rudBase{ right4, up4, direction4, base };
	glm::mat4 rud = glm::transpose(rudBase);

	glm::mat4 translation = glm::mat4(1.0f);
	translation[3] = glm::vec4(-position, 1.0f);

	return rud * translation;
}

} // namespace

glm::mat4 Camera::GetViewMatrix() const {
	return LookAt(Position, Position + Front, Up);
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
