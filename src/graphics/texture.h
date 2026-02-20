#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

struct Texture {
	unsigned int Id = 0;
	std::string Type;
	std::string Path;
};

GLuint TextureFromFile(const char* path, const std::string& directory);

