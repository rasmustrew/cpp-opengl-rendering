#pragma once
#include <glad/glad.h>

class IGpuDataLayer {

public:

	GLuint VAO{};

	virtual ~IGpuDataLayer() = default;

	virtual void setup() = 0;
	virtual void draw() = 0;
	void use() {
		glBindVertexArray(VAO);
	}

};

