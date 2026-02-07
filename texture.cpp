#include "texture.h"
#include <glad/glad.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

GLuint createBasicTexture(const char* filePath, GLuint textureIndex) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		const GLint internalFormat = static_cast<GLint>(GL_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		throw std::runtime_error("Failed to load texture: " + std::string(filePath));
	}
	stbi_image_free(data);
	return textureID;
}