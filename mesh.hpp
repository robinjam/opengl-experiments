#pragma once

#include <GL/glew.h>
#include <vector>

class mesh
{
public:
	mesh(const std::vector<GLfloat> &vertices, const std::vector<GLubyte> &indices);
	mesh(const mesh &) = delete;
	virtual ~mesh();

	mesh &operator=(const mesh &) = delete;

protected:
	GLuint vertex_array, vertex_buffer, index_buffer;
};
