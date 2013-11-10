#include "shader.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace
{
	std::string load_file(const char *filename)
	{
		std::ifstream stream(filename);
		if (!stream.is_open())
			throw std::runtime_error(std::string("Unable to open ") + filename);
		return std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	}
}

shader::shader(const char *filename, GLuint type)
{
	std::string source = load_file(filename);
	const char *c_str = source.c_str();

	name = glCreateShader(type);
	glShaderSource(name, 1, &c_str, nullptr);
	glCompileShader(name);

	GLint status;
	glGetShaderiv(name, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		glDeleteShader(name);
		throw std::runtime_error(std::string("Unable to compile ") + filename);
	}
}

shader::~shader()
{
	glDeleteShader(name);
}
