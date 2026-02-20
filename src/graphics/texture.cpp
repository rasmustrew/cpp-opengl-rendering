#include "texture.h"
#include <glad/glad.h>
#pragma warning(push, 0)
#pragma warning(disable: 6262 26819)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)
#include <stdexcept>
#include <string>

GLuint TextureFromFile(const char* path, const std::string& directory) {
	std::string filename = directory + '/' + std::string(path);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (!data) {
		throw std::runtime_error("Texture failed to load at path: " + filename);
	}

	GLenum format{};
	if (nrComponents == 1) {
		format = GL_RED;
	}
	else if (nrComponents == 3) {
		format = GL_RGB;
	}
	else if (nrComponents == 4) {
		format = GL_RGBA;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return textureID;
}
