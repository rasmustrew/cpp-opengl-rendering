#include "mesh.h"
#include "texture.h"

#include <vector>
#include <string>
#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> vertices,
		   std::vector<unsigned int> indices,
		   std::vector<Texture> textures)
	: Textures(std::move(textures)) {
	SetupMesh(vertices, indices);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(Mesh&& other) noexcept
	: Textures(std::move(other.Textures)),
	IndexCount(other.IndexCount),
	VAO(other.VAO), VBO(other.VBO), EBO(other.EBO) {
	other.IndexCount = 0;
	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
	if (this != &other) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		Textures   = std::move(other.Textures);
		IndexCount = other.IndexCount;
		VAO = other.VAO; VBO = other.VBO; EBO = other.EBO;
		other.IndexCount = 0;
		other.VAO = 0;   other.VBO = 0;   other.EBO = 0;
	}
	return *this;
}

void Mesh::SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
	IndexCount = static_cast<GLsizei>(indices.size());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)), indices.data(), GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Normal)));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, TexCoords)));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
	int diffuseNr = 0;
	int specularNr = 0;
	for (unsigned int i = 0; i < Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = Textures[i].Type;
		if (name == "diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "specular") {
			number = std::to_string(specularNr++);
		}

		shader.SetInt(("material." + name + "[" + number + "]").c_str(), static_cast<int>(i));
		glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
	}

	shader.SetInt("material.numDiffuseTextures", diffuseNr);
	shader.SetInt("material.numSpecularTextures", specularNr);
	glActiveTexture(GL_TEXTURE0);


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}