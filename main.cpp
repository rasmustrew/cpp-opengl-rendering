#include "box3D.h"
#include "rectangleColoursTexture.h"
#include "rectangleTexture.h"
#include "rectangleWrappingExperiment.h"
#include "shader.h"
#include "texture.h"
#include "textureZoomingExperiment.h"
#include "triangle.h"
#include "triangleColours.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct camera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
};




void processInput(GLFWwindow* window, camera& cam) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.position += cameraSpeed * cam.front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.position -= cameraSpeed * cam.front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.position -= glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.position += glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
}

int main() {
	try {

		GLFWwindow* window = setupWindow();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}


		glViewport(0, 0, 800, 600);

		Shader shaderProgram("mvpTransform.vert", "twoTextures.frag");
		shaderProgram.use();

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		shaderProgram.setInt("texture1", 0);
		shaderProgram.setInt("texture2", 1);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		camera cam;
		cam.position = glm::vec3(0.0f, 0.0f, 3.0f);
		cam.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cam.up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 view;
		view = glm::lookAt(cam.position, cam.position + cam.front, cam.up);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		Box3D box{};
		box.setup();

		glEnable(GL_DEPTH_TEST);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		// -------------- Render loop --------------

		while (!glfwWindowShouldClose(window)) {

			processInput(window, cam);

			shaderProgram.use();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (unsigned int i = 0; i < 10; i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i % 3 == 0) {
					angle = static_cast<float>(glfwGetTime()) * 25.0f;
				}
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shaderProgram.setMat4("model", model);
				box.draw();
			}


			view = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
			shaderProgram.setMat4("view", view);




			box.draw();


			glfwSwapBuffers(window);
			glfwPollEvents();


		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		glfwTerminate();
		return 0;
	}
}