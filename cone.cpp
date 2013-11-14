#include "cone.hpp"

#include <cmath>
#include <iostream>

namespace
{
	const GLfloat pi = atan(1) * 4;

	std::vector<GLfloat> gen_vertices(unsigned int base_vertices)
	{
		std::vector<GLfloat> result;

		result.push_back(0.f);
		result.push_back(-1.f);
		result.push_back(0.f);
		result.push_back(0.f);
		result.push_back(1.f);
		result.push_back(0.f);

		for (float i = 0; i < base_vertices; ++i)
		{
			GLfloat angle = 2 * pi * float(i) / base_vertices;
			result.push_back(cos(angle));
			result.push_back(-1.f);
			result.push_back(-sin(angle));
		}

		return result;
	}

	std::vector<GLubyte> gen_indices(unsigned int base_vertices)
	{
		std::vector<GLubyte> result;

		for (unsigned int i = 0; i < base_vertices; ++i)
		{
			// Add the triangle for the bottom face
			result.push_back(0);
			result.push_back(i % base_vertices + 2);
			result.push_back((i + 1) % base_vertices + 2);

			// Add the triangle for the side face
			result.push_back(1);
			result.push_back(i % base_vertices + 2);
			result.push_back((i + 1) % base_vertices + 2);
		}

		return result;
	}
}

cone::cone(unsigned int base_vertices)
: mesh(gen_vertices(base_vertices), gen_indices(base_vertices)), num_indices(base_vertices * 6)
{
}

void cone::draw()
{
	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_BYTE, nullptr);
	glBindVertexArray(0);
}
