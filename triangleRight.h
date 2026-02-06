#include "iGpuDataLayer.h"

class TriangleRight : public IGpuDataLayer {

	float vertices[9]{
		 1.0f,  0.5f, 0.0f,  // top right
	 1.0f, -0.5f, 0.0f,  // bottom right
	 0.0f, -0.5f, 0.0f,  // bottom left
	};
	GLuint VAO;

public:

	explicit TriangleRight(GLuint program) : IGpuDataLayer(program) {}

	void setup() override {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void draw() override {
		glUseProgram(program_);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

};