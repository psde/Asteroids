#include "Mesh.h"

#include <iostream>

namespace Geometry
{
	Mesh::Mesh(std::vector<GLfloat> vertices)
		: _vertices(vertices)
	{
		for (unsigned int i = 0; i < (vertices.size() / 2); i++)
		{
			_indices.push_back(i);
		}
		initializeMesh();
	}

	Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
		: _vertices(vertices)
		, _indices(indices)
	{
		initializeMesh();
	}

	void Mesh::initializeMesh()
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void Mesh::draw(GLenum mode)
	{
		draw(mode, _indices.size(), 0);
	}

	void Mesh::draw(GLenum mode, int count, int offset)
	{
		glBindVertexArray(_vao);
		glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		glBindVertexArray(0);
	}
}