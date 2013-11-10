#include "program.hpp"

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
