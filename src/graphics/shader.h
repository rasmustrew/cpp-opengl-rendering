#pragma once

#include "graphics/light.h"
#include "camera/camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>



class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;
	void Use();
	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetUInt(const std::string& name, unsigned int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetVec4(const std::string& name, const glm::vec4& vec) const;
	void SetUniforms(const Camera& camera, float aspectRatio) const;
	void SetUniforms(const DirectionalLight& dirLight) const;
	void SetUniforms(const SpotLight& spotLight, const Camera& cam) const;
	void SetUniforms(const PointLight& pointLight, size_t index) const;
private:
	unsigned int ID = 0;
};


