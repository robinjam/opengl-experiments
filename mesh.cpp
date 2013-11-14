#include "mesh.hpp"

mesh::mesh(const std::vector<GLfloat> &vertices, const std::vector<GLubyte> &indices)
{
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// No need to unbind GL_ELEMENT_ARRAY_BUFFER since it is encapsulated by the VAO
	glBindVertexArray(0);
}

mesh::~mesh()
{
	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &index_buffer);
}
