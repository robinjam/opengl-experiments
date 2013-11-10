#pragma once

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

class shader
{
	friend class program;

public:
	shader(const char *filename, GLuint type);
	shader(const shader &) = delete;
	~shader();

	shader &operator=(const shader &) = delete;

private:
	GLuint name;
};
