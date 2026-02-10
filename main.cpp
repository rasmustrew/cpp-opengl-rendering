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


float cubeShininess[] = {
	0.6f * 128, // emerald
	0.1f * 128, // jade
	0.3f * 128, // obsidian
	0.088f * 128, // pearl
	0.6f * 128, // ruby
	0.25f * 128, // white plastic
	0.25f * 128, // cyan plastic
	0.078125f * 128, // green rubber
	0.078125f * 128, // red rubber
	0.078125f * 128 // black rubber
};

void drawLightSource(Shader& lightSourceShader, Camera& cam, LightSourceCube& lightCube, pointLight& light) {
	lightSourceShader.use();
	lightSourceShader.setMat4("view", cam.GetViewMatrix());
	lightSourceShader.setMat4("projection", cam.GetProjectionMatrix(WINDOW_ASPECT_RATIO));
	lightSourceShader.setMat4("model", light.model);
	lightSourceShader.setVec4("ourColor", glm::vec4(light.diffuse, 1.0f));
	lightCube.use();
	lightCube.draw();
}

void drawBoxes(Shader& objectShader, std::array<object3D, 10>& objects, CameraFps& cam, IGpuDataLayer& box, spotLight& spotLight, std::array<pointLight, 4>& lights) {
	objectShader.use();
	objectShader.setMat4("view", cam.GetViewMatrix());
	objectShader.setMat4("projection", cam.GetProjectionMatrix(WINDOW_ASPECT_RATIO));
	objectShader.setVec3("viewPos", cam.Position);
	setSpotLightUniforms(objectShader, spotLight, cam);

	for (size_t i = 0; i < lights.size(); i++) {
		auto& light = lights[i];
		setPointLightUniforms(objectShader, light, i);
	}


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


		Shader lightSourceShader("mvpWithTexture.vert", "colorUniform.frag");
		directionalLight directionalLight = getDefaultDirectionalLight();
		spotLight spotLight = getDefaultSpotLight();


		std::array<pointLight, 4> lights{};

		for (size_t i = 0; i < lights.size(); i++) {
			lights[i] = getDefaultPointLight();
			lights[i].model = glm::translate(lights[i].model, pointLightPositions[i]);
			lights[i].model = glm::scale(lights[i].model, glm::vec3(0.2f));
		}


		Shader objectShader("mvpNormalTexture.vert", "multipleLights.frag");
		objectShader.use();

		setDirectionalLightUniforms(objectShader, directionalLight);

		createBasicTexture("resources/container2.png", 0);
		createBasicTexture("resources/container2_specular.png", 1);
		objectShader.setInt("material.diffuse", 0);
		objectShader.setInt("material.specular", 1);

		std::array<object3D, 10> objects{};

		for (size_t i = 0; i < objects.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			material mat{};
			mat.shininess = cubeShininess[i];
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

			for (auto& light : lights) {
				drawLightSource(lightSourceShader, cam, lightCube, light);
			}


			drawBoxes(objectShader, objects, cam, box, spotLight, lights);

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