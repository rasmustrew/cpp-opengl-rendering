#include "box3D.h"
#include "camera.h"
#include "input.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct mvpMatrices {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

mvpMatrices createMvpMatrices(const Camera& cam) {
	mvpMatrices matrices;
	matrices.model = glm::mat4(1.0f);
	matrices.model = glm::mat4(1.0F);
	matrices.view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
	matrices.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
	return matrices;
}

float getTime() {
	return static_cast<float>(glfwGetTime());
}



int main() {
	try {

		Camera cam{};
		mouseState initialMouseState{ };
		windowCallbackData callbackData{ cam, initialMouseState };
		GLFWwindow* window = setupWindow();
		setupInput(window, callbackData);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


		Shader shaderProgram("mvpTransform.vert", "twoTextures.frag");
		shaderProgram.use();

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		shaderProgram.setInt("texture1", 0);
		shaderProgram.setInt("texture2", 1);

		mvpMatrices mvp = createMvpMatrices(cam);
		shaderProgram.setMat4("model", mvp.model);
		shaderProgram.setMat4("view", mvp.view);
		shaderProgram.setMat4("projection", mvp.projection);

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

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		while (!glfwWindowShouldClose(window)) {

			float currentFrame = getTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processKeyboardInput(window, cam, deltaTime);

			shaderProgram.use();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (unsigned int i = 0; i < 10; i++) {
				mvp.model = glm::mat4(1.0f);
				mvp.model = glm::translate(mvp.model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i % 3 == 0) {
					angle = getTime() * 25.0f;
				}
				mvp.model = glm::rotate(mvp.model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shaderProgram.setMat4("model", mvp.model);
				box.draw();
			}


			mvp.view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
			mvp.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, 0.1f, 100.0f);
			shaderProgram.setMat4("view", mvp.view);
			shaderProgram.setMat4("projection", mvp.projection);

			box.draw();


			glfwSwapBuffers(window);
			glfwPollEvents();


		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	glfwTerminate();
	return 0;
}