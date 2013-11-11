#pragma once

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <initializer_list>
#include "shader.hpp"

class program
{
public:
	program(std::initializer_list<shader *> shaders);
	program(const program &) = delete;
	~program();

	void use();
	void set_uniform(const char *name, glm::mat4 &value);

	program &operator=(const program &) = delete;

private:
	GLuint name;
};
