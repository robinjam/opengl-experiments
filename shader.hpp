#pragma once

#include <GL/glew.h>

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
