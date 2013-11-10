#pragma once

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <initializer_list>
#include "shader.hpp"

class program
{
public:
	program(std::initializer_list<shader *> shaders);
	program(const program &) = delete;
	~program();

	void use();

	program &operator=(const program &) = delete;

private:
	GLuint name;
};
