#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>


namespace {

	std::string ReadShaderSource(const char* filepath) {
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(filepath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			return shaderStream.str();
		}
		catch (const std::ifstream::failure&) {
			throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " + std::string(filepath));
		}
	}

	GLuint CreateShader(GLenum shaderType, const char* source) {
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLint logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<size_t>(logLength), '\0');
			glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
			glDeleteShader(shader);
			throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED\n" + infoLog);
		}
		return shader;
	}

	GLuint CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			GLint logLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			std::string infoLog(static_cast<size_t>(logLength), '\0');
			glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());
			glDeleteProgram(program);
			throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return program;
	}

}


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode = ReadShaderSource(vertexPath);
	std::string fragmentCode = ReadShaderSource(fragmentPath);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexCode.c_str());
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	ID = CreateShaderProgram(vertexShader, fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

Shader::Shader(Shader&& other) noexcept : ID(other.ID) {
	other.ID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
	if (this != &other) {
		glDeleteProgram(ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}


void Shader::Use() {
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUInt(const std::string& name, unsigned int value) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1ui(location, value);
}

void Shader::SetFloat(const std::string& name, float value) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& vec) const {
	const GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Shader::SetUniforms(const Camera& camera, float aspectRatio) const {
	SetVec3("viewPos", camera.GetPosition());
	SetMat4("view", camera.GetViewMatrix());
	SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
}

void Shader::SetUniforms(const DirectionalLight& dirLight) const {
	SetVec3("dirLight.direction", dirLight.Direction);
	SetVec3("dirLight.ambient", dirLight.Ambient);
	SetVec3("dirLight.diffuse", dirLight.Diffuse);
	SetVec3("dirLight.specular", dirLight.Specular);
}

void Shader::SetUniforms(const SpotLight& spotLight, const Camera& cam) const {
	SetVec3("spotLight.ambient", spotLight.Ambient);
	SetVec3("spotLight.diffuse", spotLight.Diffuse);
	SetVec3("spotLight.specular", spotLight.Specular);
	SetFloat("spotLight.cutOff", spotLight.CutOff);
	SetFloat("spotLight.outerCutOff", spotLight.OuterCutOff);
	SetVec3("spotLight.position", cam.GetPosition());
	SetVec3("spotLight.direction", cam.GetFront());
}
void Shader::SetUniforms(const PointLight& pointLight, size_t index) const {
	std::string prefix = "pointLights[" + std::to_string(index) + "]";
	SetVec3(prefix + ".position", pointLight.Position);
	SetVec3(prefix + ".ambient", pointLight.Ambient);
	SetVec3(prefix + ".diffuse", pointLight.Diffuse);
	SetVec3(prefix + ".specular", pointLight.Specular);
	SetFloat(prefix + ".constant", pointLight.Constant);
	SetFloat(prefix + ".linear", pointLight.Linear);
	SetFloat(prefix + ".quadratic", pointLight.Quadratic);
}