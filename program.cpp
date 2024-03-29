#include "program.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

program::program(std::initializer_list<shader *> shaders)
{
	name = glCreateProgram();

	for (auto shader : shaders)
		glAttachShader(name, shader->name);

	glLinkProgram(name);

	// It is safe to detach the shaders after the program has been linked
	// http://www.opengl.org/sdk/docs/man3/xhtml/glLinkProgram.xml
	for (auto shader : shaders)
		glDetachShader(name, shader->name);

	GLint status;
	glGetProgramiv(name, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLchar log[512];
		glGetProgramInfoLog(name, 512, nullptr, log);
		std::cerr << log << std::endl;
		glDeleteProgram(name);
		throw std::runtime_error("Unable to link shader program");
	}
}

program::~program()
{
	glDeleteProgram(name);
}

void program::use()
{
	glUseProgram(name);
}

void program::set_uniform(const char *name, const glm::vec4 &value)
{
	use();

	GLint location = glGetUniformLocation(this->name, name);
	if (location != -1)
		glUniform4fv(location, 1, glm::value_ptr(value));
}

void program::set_uniform(const char *name, const glm::mat4 &value)
{
	use();

	GLint location = glGetUniformLocation(this->name, name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
