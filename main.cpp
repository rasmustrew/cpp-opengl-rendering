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

		Shader shaderProgram("matrixTransform.vert", "twoTextures.frag");
		shaderProgram.use();

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		shaderProgram.setInt("texture1", 0);
		shaderProgram.setInt("texture2", 1);





		RectangleTexture rectangle{};
		rectangle.setup();





		// -------------- Render loop --------------

		while (!glfwWindowShouldClose(window)) {

			processInput(window);


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
			shaderProgram.setMat4("transform", trans);

			rectangle.draw();

			glm::mat4 trans2 = glm::mat4(1.0f);
			trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
			trans2 = glm::scale(trans2, glm::vec3((sin((float)glfwGetTime()) + 1.0f) / 2.0f));

			shaderProgram.setMat4("transform", trans2);
			rectangle.draw();

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