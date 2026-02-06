#pragma once
#include <glad/glad.h>

class IGpuDataLayer {

public:

	virtual ~IGpuDataLayer() = default;

	virtual void setup() = 0;
	virtual void draw() = 0;

};

