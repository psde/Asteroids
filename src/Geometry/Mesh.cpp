#include "Mesh.h"

#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>

namespace Geometry
{
	Mesh::Mesh(std::vector<glm::vec2> vertices)
	: _vertices(vertices)
	, _initialized(false)
	{
		for (decltype(vertices)::size_type i = 0; i < vertices.size(); ++i)
		{
			_indices.push_back(i);
		}
	}

	Mesh::Mesh(std::vector<glm::vec2> vertices, std::vector<GLuint> indices)
	: _vertices(vertices)
	, _indices(indices)
	, _initialized(false)
	{
	}

	Mesh::~Mesh()
	{
		deleteMesh();
	}

	void Mesh::deleteMesh()
	{
		if (!_initialized)
			return;
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
		_initialized = false;
	}

	void Mesh::initializeMesh()
	{
		if (_initialized)
			return;

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec2), _vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		_initialized = true;
	}

	void Mesh::draw(GLenum mode)
	{
		draw(mode, _indices.size(), 0);
	}

	void Mesh::draw(GLenum mode, int count, int offset)
	{
		initializeMesh();
		glBindVertexArray(_vao);
		glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		glBindVertexArray(0);
	}

	glm::vec4 Mesh::getBoundingBox()
	{
		glm::vec4 bb;
		
		for (auto &v : _vertices)
		{
			bb.x = std::min(bb.x, v.x);
			bb.y = std::min(bb.y, v.y);
			bb.z = std::max(bb.z, v.x);
			bb.w = std::max(bb.w, v.y);
		}

		return bb;
	}

	std::unique_ptr<Mesh> Mesh::rotate(float rotation, glm::vec2 center)
	{
		auto vertices = _vertices;

		for (auto &v : vertices)
		{
			v -= center;
			v = glm::rotate(v, rotation);
			v += center;
		}

		return std::unique_ptr<Mesh>(new Mesh(vertices, _indices));
	}
}