#pragma once

#include "texture.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void Draw(Shader& shader);
private:
	std::vector<Texture> Textures;
	GLsizei IndexCount = 0;
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

