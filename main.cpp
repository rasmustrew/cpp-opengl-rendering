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






void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);

		Box3D box{};
		box.setup();

		glEnable(GL_DEPTH_TEST);



		// -------------- Render loop --------------

		while (!glfwWindowShouldClose(window)) {

			processInput(window);


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderProgram.use();

			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			shaderProgram.setMat4("model", model);

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