#include "shader.h"
#include "triangle.h"
#include "triangleColours.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};



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

		Shader shaderProgram("positionOut.vert", "positionInput.frag");

		TriangleColours triangle{};
		triangle.setup();


		// -------------- Element Buffer Object (EBO) setup --------------
		unsigned int EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		// -------------- Render loop --------------

		while (!glfwWindowShouldClose(window)) {

			processInput(window);


			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();
			triangle.draw();

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