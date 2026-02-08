#include "boxNormal.h"
#include "cameraFps.h"
#include "cameraGlide.h"
#include "input.h"
#include "lightSourceCube.h"
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
	matrices.view = cam.GetViewMatrix();
	matrices.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);
	return matrices;
}

float getTime() {
	return static_cast<float>(glfwGetTime());
}



int main() {
	try {

		CameraFps cam{};
		mouseState initialMouseState{ };
		windowCallbackData callbackData{ cam, initialMouseState };
		GLFWwindow* window = setupWindow();
		setupInput(window, callbackData);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


		mvpMatrices mvp = createMvpMatrices(cam);


		Shader lightSourceShader("mvpWithTexture.vert", "lightSource.frag");
		lightSourceShader.use();
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		glm::mat4 model = model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMat4("model", model);
		//lightSourceShader.setVec3("lightPos", lightPos);


		Shader objectShader("mvpWithNormal.vert", "ambientDiffuseSpecular.frag");
		objectShader.use();
		objectShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		objectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.setVec3("lightPos", lightPos);

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		objectShader.setInt("texture1", 0);
		objectShader.setInt("texture2", 1);


		BoxNormal box{};
		box.setup();
		LightSourceCube light{};
		light.setup();

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

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			float currentFrame = getTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processKeyboardInput(window, cam, deltaTime);

			mvp.view = cam.GetViewMatrix();
			mvp.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, 0.1f, 100.0f);


			lightSourceShader.use();
			lightSourceShader.setMat4("view", mvp.view);
			lightSourceShader.setMat4("projection", mvp.projection);
			light.use();
			light.draw();

			objectShader.use();
			objectShader.setMat4("view", mvp.view);
			objectShader.setMat4("projection", mvp.projection);
			objectShader.setVec3("viewPos", cam.Position);

			box.use();

			for (unsigned int i = 0; i < 10; i++) {
				mvp.model = glm::mat4(1.0f);
				mvp.model = glm::translate(mvp.model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i % 3 == 0) {
					angle = getTime() * 25.0f;
				}
				mvp.model = glm::rotate(mvp.model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				objectShader.setMat4("model", mvp.model);
				glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mvp.model)));
				objectShader.setMat3("normalMatrix", normalMatrix);
				box.draw();
			}






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