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

void setupGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
}

void drawLightSource(Shader& lightSourceShader, mvpMatrices& mvp, LightSourceCube& light, const glm::mat4& lightModelBase, glm::vec3& lightPos) {
	lightSourceShader.use();
	lightSourceShader.setMat4("view", mvp.view);
	lightSourceShader.setMat4("projection", mvp.projection);
	light.use();

	float lightTranslation = 1.0f + sin(getTime()) * 20.0f;
	glm::vec3 lightXTranslation = glm::vec3(lightTranslation, 0.0f, 0.0f);
	glm::mat4 lightModel = lightModelBase;
	lightModel = glm::translate(lightModel, lightXTranslation);
	lightPos = glm::vec3(lightModel[3]);

	lightSourceShader.setMat4("model", lightModel);

	light.draw();
}

void DrawBoxes(Shader& objectShader, mvpMatrices& mvp, CameraFps& cam, glm::vec3& lightPos, BoxNormal& box) {
	objectShader.use();
	objectShader.setMat4("view", mvp.view);
	objectShader.setMat4("projection", mvp.projection);
	objectShader.setVec3("viewPos", cam.Position);
	objectShader.setVec3("light.position", lightPos);


	glm::vec3 lightColor;
	lightColor.x = sin(getTime() * 2.0f);
	lightColor.y = sin(getTime() * 0.7f);
	lightColor.z = sin(getTime() * 1.3f);

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

	objectShader.setVec3("light.ambient", ambientColor);
	objectShader.setVec3("light.diffuse", diffuseColor);


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


		mvpMatrices mvp = createMvpMatrices(cam);


		Shader lightSourceShader("mvpWithTexture.vert", "lightSource.frag");
		lightSourceShader.use();
		glm::vec3 lightPos(-1.2f, 1.0f, 2.0f);
		glm::mat4 lightModelBase = glm::mat4(1.0f);
		lightModelBase = glm::translate(lightModelBase, lightPos);
		lightModelBase = glm::scale(lightModelBase, glm::vec3(0.2f));



		Shader objectShader("mvpWithNormal.vert", "singleMaterial.frag");
		objectShader.use();
		objectShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		objectShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		objectShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		objectShader.setFloat("material.shininess", 32.0f);

		objectShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		objectShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		objectShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		createBasicTexture("resources/container.jpg", 0);
		createBasicTexture("resources/awesomeface.png", 1);
		objectShader.setInt("texture1", 0);
		objectShader.setInt("texture2", 1);


		BoxNormal box{};
		box.setup();
		LightSourceCube light{};
		light.setup();

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

			mvp.view = cam.GetViewMatrix();
			mvp.projection = glm::perspective(glm::radians(cam.Zoom), WINDOW_ASPECT_RATIO, 0.1f, 100.0f);

			drawLightSource(lightSourceShader, mvp, light, lightModelBase, lightPos);
			DrawBoxes(objectShader, mvp, cam, lightPos, box);

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