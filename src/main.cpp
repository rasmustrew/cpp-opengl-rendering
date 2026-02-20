#include "camera/cameraFps.h"
#include "core/input.h"
#include "core/util.h"
#include "core/window.h"
#include "graphics/light.h"
#include "graphics/lightSourceCube.h"
#include "graphics/model.h"
#include "graphics/shader.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include <array>



namespace {

void RenderLightSource(Shader& lightSourceShader, Camera& cam, LightSourceCube& lightCube, const PointLight& light, float aspectRatio) {
	glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), light.Position), glm::vec3(0.2f));
	lightSourceShader.Use();
	lightSourceShader.SetMat4("view", cam.GetViewMatrix());
	lightSourceShader.SetMat4("projection", cam.GetProjectionMatrix(aspectRatio));
	lightSourceShader.SetMat4("model", model);
	lightSourceShader.SetVec4("ourColor", glm::vec4(light.Diffuse, 1.0f));
	lightCube.Draw();
}

} // namespace

int main() {
	try {

		/// -------------- Platform Setup --------------
		stbi_set_flip_vertically_on_load(true);

		CameraFps cam{};
		MouseState initialMouseState{ };
		WindowCallbackData callbackData{ cam, initialMouseState };
		GLFWwindow* window = SetupWindow();
		SetupInput(window, callbackData);

		SetupGlad();

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// -------------- Scene Setup --------------


		Shader lightSourceShader("shaders/mvp.vert", "shaders/colorUniform.frag");
		Shader objectShader("shaders/mvpNormalTexture.vert", "shaders/mesh.frag");

		DirectionalLight directionalLight{};
		SpotLight spotLight{};
		LightSourceCube lightCube{ };
		lightCube.Setup();


		std::vector<Texture> textureCache;
		Model backpack("resources/backpack/backpack.obj", textureCache);

		std::array<PointLight, 4> lights{};
		const glm::vec3 pointLightPositions[] = {
			glm::vec3( 0.7f,  0.2f,  2.0f),
			glm::vec3( 2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3( 0.0f,  0.0f, -3.0f)
		};
		for (size_t i = 0; i < lights.size(); i++) {
			lights[i].Position = pointLightPositions[i];
		}


		// -------------- Static Uniform Configuration --------------

		objectShader.Use();

		objectShader.SetFloat("shininess", 32.0f);
		glm::mat4 model = glm::mat4(1.0f);
		objectShader.SetMat4("model", model);
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
		objectShader.SetMat3("normalMatrix", normalMatrix);
		objectShader.SetUniforms(directionalLight);


		// -------------- Render loop --------------

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		while (!glfwWindowShouldClose(window)) {

			// -------------- Reset Frame --------------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// -------------- Timing --------------
			float currentFrame = GetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			int fbWidth, fbHeight;
			glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
			const float aspectRatio = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

			// -------------- Input --------------
			ProcessKeyboardInput(window, cam, deltaTime);


			// -------------- Rendering State --------------
			objectShader.Use();

			for (size_t i = 0; i < lights.size(); i++) {
				objectShader.SetUniforms(lights[i], i);
			}
			objectShader.SetInt("numPointLights", static_cast<int>(lights.size()));
			objectShader.SetUniforms(cam, aspectRatio);
			objectShader.SetUniforms(spotLight, cam);
			// -------------- Render Scene --------------

			backpack.Draw(objectShader);

			for (auto& light : lights) {
				RenderLightSource(lightSourceShader, cam, lightCube, light, aspectRatio);
			}

			// ------------- Frame end --------------

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