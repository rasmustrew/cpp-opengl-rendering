#include "rectangleColoursTexture.h"
#include "rectangleWrappingExperiment.h"
#include "shader.h"
#include "texture.h"
#include "textureZoomingExperiment.h"
#include "triangle.h"
#include "triangleColours.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>






void processInput(GLFWwindow* window, float& mixValue) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mixValue = std::min(1.0f, mixValue + 0.01f);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mixValue = std::max(0.0f, mixValue - 0.01f);
}

int main() {
	try {

		GLFWwindow* window = setupWindow();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}


		glViewport(0, 0, 800, 600);

		Shader shaderProgram("colorTexture.vert", "texturesVariableMix.frag");
		shaderProgram.use();

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		shaderProgram.setInt("texture1", 0);
		shaderProgram.setInt("texture2", 1);
		float mixValue = 0.5f;


		RectangleColoursTexture rectangle{};
		rectangle.setup();





		// -------------- Render loop --------------

		while (!glfwWindowShouldClose(window)) {

			processInput(window, mixValue);


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();
			shaderProgram.setFloat("mixValue", mixValue);
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