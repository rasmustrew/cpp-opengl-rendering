#include "boxNormal.h"
#include "cameraFps.h"
#include "cameraGlide.h"
#include "input.h"
#include "light.h"
#include "lightSourceCube.h"
#include "Object3D.h"
#include "shader.h"
#include "texture.h"
#include "util.h"
#include "window.h"
#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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

glm::vec3 cubeAmbient[] = {
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f),
	glm::vec3(1.0f, 1.0f, 1.0f) * glm::vec3(0.2f)
};

glm::vec3 cubeDiffuse[] = {
	glm::vec3(1.0f, 0.5f, 0.31f),
	glm::vec3(0.5f, 1.0f, 0.31f),
	glm::vec3(0.5f, 0.5f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.31f),
	glm::vec3(1.0f, 0.5f, 1.0f),
	glm::vec3(0.5f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	glm::vec3(1.0f, 0.5f, 0.5f),
	glm::vec3(0.5f, 1.0f, 0.5f)
};





void drawLightSource(Shader& lightSourceShader, Camera& cam, LightSourceCube& lightCube, light& light) {
	lightSourceShader.use();
	lightSourceShader.setMat4("view", cam.GetViewMatrix());
	lightSourceShader.setMat4("projection", cam.GetProjectionMatrix(WINDOW_ASPECT_RATIO));
	lightSourceShader.setMat4("model", light.model);
	lightSourceShader.setVec4("ourColor", glm::vec4(light.diffuse, 1.0f));
	lightCube.use();
	lightCube.draw();
}

void drawBoxes(Shader& objectShader, std::array<object3D, 10>& objects, CameraFps& cam, BoxNormal& box, light& light) {
	objectShader.use();
	objectShader.setMat4("view", cam.GetViewMatrix());
	objectShader.setMat4("projection", cam.GetProjectionMatrix(WINDOW_ASPECT_RATIO));
	objectShader.setVec3("viewPos", cam.Position);

	objectShader.setVec3("light.position", glm::vec3(light.model[3]));
	objectShader.setVec3("light.ambient", light.ambient);
	objectShader.setVec3("light.diffuse", light.diffuse);
	objectShader.setVec3("light.specular", light.specular);


	box.use();

	for (unsigned int i = 0; i < objects.size(); i++) {
		object3D& obj = objects[i];
		obj.model = glm::mat4(1.0f);
		obj.model = glm::translate(obj.model, cubePositions[i]);
		float angle = 20.0f * i;
		if (i % 3 == 0) {
			angle = getTime() * 25.0f;
		}
		obj.model = glm::rotate(obj.model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		objectShader.setMat4("model", obj.model);
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(obj.model)));
		objectShader.setMat3("normalMatrix", normalMatrix);

		objectShader.setVec3("material.ambient", obj.mat.ambient);
		objectShader.setVec3("material.diffuse", obj.mat.diffuse);
		objectShader.setVec3("material.specular", obj.mat.specular);
		objectShader.setFloat("material.shininess", obj.mat.shininess);

		box.draw();
	}
}

int main() {
	try {

		CameraFps cam{};
		mouseState initialMouseState{ };
		windowCallbackData callbackData{ cam, initialMouseState };
		GLFWwindow* window = setupWindow();
		setupInput(window, callbackData);

		setupGlad();

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		light light = getDefaultLight();
		Shader lightSourceShader("mvpWithTexture.vert", "colorUniform.frag");


		Shader objectShader("mvpWithNormal.vert", "singleMaterial.frag");
		objectShader.use();


		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		objectShader.setInt("texture1", 0);
		objectShader.setInt("texture2", 1);

		std::array<object3D, 10> objects{};

		for (size_t i = 0; i < objects.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			material mat{
				cubeAmbient[i],
				cubeDiffuse[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				32.0f
			};
			objects[i] = { model, mat };
		}


		BoxNormal box{};
		box.setup();
		LightSourceCube lightCube{ };
		lightCube.setup();

		glEnable(GL_DEPTH_TEST);


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

			updateLight(light);



			drawLightSource(lightSourceShader, cam, lightCube, light);
			drawBoxes(objectShader, objects, cam, box, light);

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