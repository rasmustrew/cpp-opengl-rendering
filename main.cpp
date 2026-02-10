#include "boxNormal.h"
#include "boxNormalTextured.h"
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
			glm::vec3(0.0f,  0.0f,  0.0f), // emerald
			glm::vec3(2.0f,  5.0f, -15.0f), // jade
			glm::vec3(-1.5f, -2.2f, -2.5f), // obsidian
			glm::vec3(-3.8f, -2.0f, -12.3f), // pearl
			glm::vec3(2.4f, -0.4f, -3.5f), // ruby
			glm::vec3(-1.7f,  3.0f, -7.5f), // white plastic
			glm::vec3(1.3f, -2.0f, -2.5f), // cyan plastic
			glm::vec3(1.5f,  2.0f, -2.5f), // green rubber
			glm::vec3(1.5f,  0.2f, -1.5f), // red rubber
			glm::vec3(-1.3f,  1.0f, -1.5f) // black rubber
};

glm::vec3 cubeAmbient[] = {
	glm::vec3(0.0215f, 0.1745f, 0.0215f), // emerald
	glm::vec3(0.135f, 0.2225f, 0.1575f), // jade
	glm::vec3(0.05375f, 0.05f, 0.06625f), // obsidian
	glm::vec3(0.25f, 0.20725f, 0.20725f), // pearl
	glm::vec3(0.1745f, 0.01175f, 0.01175f), // ruby
	glm::vec3(0.0f, 0.0f, 0.0f), // white plastic
	glm::vec3(0.0f, 0.1f, 0.06f), // cyan plastic
	glm::vec3(0.0f, 0.05f, 0.0f), // green rubber
	glm::vec3(0.05f, 0.0f, 0.0f), // red rubber
	glm::vec3(0.02f, 0.02f, 0.02f)  // black rubber
};

glm::vec3 cubeDiffuse[] = {
	glm::vec3(0.07568f, 0.61424f, 0.07568f), // emerald
	glm::vec3(0.54f, 0.89f, 0.63f), // jade
	glm::vec3(0.18275f, 0.17f, 0.22525f), // obsidian
	glm::vec3(1.0f, 0.829f, 0.829f), // pearl
	glm::vec3(0.61424f, 0.04136f, 0.04136f), // ruby
	glm::vec3(0.55f, 0.55f, 0.55f), // white plastic
	glm::vec3(0.0f, 0.50980392f, 0.50980392f), // cyan plastic
	glm::vec3(0.4f, 0.5f, 0.4f), // green rubber
	glm::vec3(0.5f, 0.4f, 0.4f), // red rubber
	glm::vec3(0.01f, 0.01f, 0.01f)  // black rubber
};

glm::vec3 cubeSpecular[] = {
	glm::vec3(0.633f, 0.727811f, 0.633f), // emerald
	glm::vec3(0.316228f, 0.316228f, 0.316228f), // jade
	glm::vec3(0.332741f, 0.328634f, 0.346435f), // obsidian
	glm::vec3(0.296648f, 0.296648f, 0.296648f), // pearl
	glm::vec3(0.727811f, 0.626959f, 0.626959f), // ruby
	glm::vec3(0.70f, 0.70f, 0.70f), // white plastic
	glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), // cyan plastic
	glm::vec3(0.04f, 0.7f, 0.04f), // green rubber
	glm::vec3(0.7f, 0.04f, 0.04f), // red rubber
	glm::vec3(0.4f, 0.4f, 0.4f)  // black rubber
};

float cubeShininess[] = {
	0.6f * 128,
	0.1f * 128,
	0.3f * 128,
	0.088f * 128,
	0.6f * 128,
	0.25f * 128,
	0.25f * 128,
	0.078125f * 128,
	0.078125f * 128,
	0.078125f * 128
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

void drawBoxes(Shader& objectShader, std::array<object3D, 10>& objects, CameraFps& cam, IGpuDataLayer& box, light& light) {
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


		Shader objectShader("mvpNormalTexture.vert", "materialMaps.frag");
		objectShader.use();


		createBasicTexture("resources/container2.png", 0);
		createBasicTexture("resources/container2_specular.png", 1);
		objectShader.setInt("material.diffuse", 0);
		objectShader.setInt("material.specular", 1);

		std::array<object3D, 10> objects{};

		for (size_t i = 0; i < objects.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			material mat{
				cubeAmbient[i],
				cubeDiffuse[i],
				cubeSpecular[i],
				cubeShininess[i]
			};
			objects[i] = { model, mat };
		}


		BoxNormalTextured box{};
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