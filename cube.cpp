#include "cube.hpp"

#include <cmath>

namespace
{
	const GLfloat a = 1 / sqrt(3);

	const std::vector<GLfloat> vertices = {
		 a,  a,  a,
		-a,  a,  a,
		-a, -a,  a,
		 a, -a,  a,
		 a, -a, -a,
		 a,  a, -a,
		-a,  a, -a,
		-a, -a, -a
	};

	const std::vector<GLubyte> indices = {
		0, 1, 2, 3,
		0, 3, 4, 5,
		0, 5, 6, 1,
		1, 6, 7, 2,
		7, 4, 3, 2,
		4, 7, 6, 5
	};
}

cube::cube()
: mesh(vertices, indices)
{
}

void cube::draw()
{
	glBindVertexArray(vertex_array);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, 24, GL_UNSIGNED_BYTE, nullptr);
	glBindVertexArray(0);
}
