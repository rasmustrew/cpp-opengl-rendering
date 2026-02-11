#include "boxNormal.h"
#include "boxNormalTextured.h"
#include "cameraFps.h"
#include "cameraGlide.h"
#include "input.h"
#include "light.h"
#include "lightSourceCube.h"
#include "model.h"
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
#include <stb_image.h>

void drawLightSource(Shader& lightSourceShader, Camera& cam, LightSourceCube& lightCube, pointLight& light) {
	lightSourceShader.use();
	lightSourceShader.setMat4("view", cam.GetViewMatrix());
	lightSourceShader.setMat4("projection", cam.GetProjectionMatrix(WINDOW_ASPECT_RATIO));
	lightSourceShader.setMat4("model", light.model);
	lightSourceShader.setVec4("ourColor", glm::vec4(light.diffuse, 1.0f));
	lightCube.use();
	lightCube.draw();
}

int main() {
	try {
		stbi_set_flip_vertically_on_load(true);

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

		Model backpack("resources/backpack/backpack.obj");

		Shader objectShader("mvpNormalTexture.vert", "mesh.frag");
		objectShader.use();

		objectShader.setFloat("shininess", 32.0f);
		setDirectionalLightUniforms(objectShader, directionalLight);
		glm::mat4 model = glm::mat4(1.0f);
		objectShader.setMat4("model", model);
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
		objectShader.setMat3("normalMatrix", normalMatrix);





		LightSourceCube lightCube{ };
		lightCube.setup();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


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

			objectShader.use();
			for (size_t i = 0; i < lights.size(); i++) {
				auto& light = lights[i];
				setPointLightUniforms(objectShader, light, i);
			}
			objectShader.setInt("numPointLights", static_cast<int>(lights.size()));

			cam.SetUniforms(objectShader);
			setSpotLightUniforms(objectShader, spotLight, cam);



			backpack.Draw(objectShader);

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