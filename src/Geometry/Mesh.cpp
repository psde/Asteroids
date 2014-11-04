#include "Mesh.h"

//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/glm.hpp>
#include "Math/Math.h"

#include <iostream>
#include <algorithm>

namespace Geometry
{
	Mesh::Mesh(const Mesh &other)
		: _initialized(false)
		, _vertices(other._vertices)
		, _indices(other._indices)
		, _boundingBox(other._boundingBox)
	{ }

	Mesh::Mesh(std::vector<glm::vec2> vertices)
		: _initialized(false)
		, _vertices(vertices)
	{
		for (decltype(vertices)::size_type i = 0; i < vertices.size(); ++i)
		{
			_indices.push_back(i);
		}
		createBoundingBox();
	}

	Mesh::Mesh(std::vector<glm::vec2> vertices, std::vector<GLuint> indices)
		: _initialized(false)
		, _vertices(vertices)
		, _indices(indices)
	{
		createBoundingBox();
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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glGenBuffers(1, &_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		_initialized = true;
	}

	void Mesh::createBoundingBox()
	{
		// Create bounding box
		for (auto &v : _vertices)
		{
			_boundingBox.x = std::min(_boundingBox.x, v.x);
			_boundingBox.y = std::min(_boundingBox.y, v.y);
			_boundingBox.z = std::max(_boundingBox.z, v.x);
			_boundingBox.w = std::max(_boundingBox.w, v.y);
		}
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
		return _boundingBox;
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

	std::unique_ptr<Mesh> Mesh::scale(glm::vec2 scaling)
	{
		auto vertices = _vertices;

		for (auto &v : vertices)
		{
			v *= scaling;
		}

		return std::unique_ptr<Mesh>(new Mesh(vertices, _indices));
	}
}