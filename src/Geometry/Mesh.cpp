#include "Mesh.h"

namespace Geometry
{

	Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
	{
		_vertices = vertices;
		_indices = indices;

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	void Mesh::setup(Shader::Program *program)
	{

	}

	void Mesh::draw()
	{
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}