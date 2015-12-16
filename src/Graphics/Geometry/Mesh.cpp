#include "Mesh.h"

#include "Math/Math.h"

#include <iostream>
#include <algorithm>

namespace Graphics
{
	Mesh::Mesh(const Mesh &other)
		: m_initialized(false)
		, m_vertices(other.m_vertices)
		, m_indices(other.m_indices)
		, m_boundingBox(other.m_boundingBox)
	{ }

	Mesh::Mesh(std::vector<Math::vec2> vertices)
		: m_initialized(false)
		, m_vertices(vertices)
	{
		for (decltype(vertices)::size_type i = 0; i < vertices.size(); ++i)
		{
			m_indices.push_back(i);
		}
		createBoundingBox();
	}

	Mesh::Mesh(std::vector<Math::vec2> vertices, std::vector<GLuint> indices)
		: m_initialized(false)
		, m_vertices(vertices)
		, m_indices(indices)
	{
		createBoundingBox();
	}

	Mesh::~Mesh()
	{
		deleteMesh();
	}

	void Mesh::deleteMesh()
	{
		if (!m_initialized)
			return;
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
		glDeleteVertexArrays(1, &m_vao);
		m_initialized = false;
	}

	void Mesh::initializeMesh()
	{
		if (m_initialized)
			return;
		
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Math::vec2), m_vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

		glPrimitiveRestartIndex(GeometryRestartIndex);

		glBindVertexArray(0);

		m_initialized = true;
	}

	void Mesh::createBoundingBox()
	{
		// Create bounding box
		for (auto &v : m_vertices)
		{
			m_boundingBox.x = std::min(m_boundingBox.x, v.x);
			m_boundingBox.y = std::min(m_boundingBox.y, v.y);
			m_boundingBox.z = std::max(m_boundingBox.z, v.x);
			m_boundingBox.w = std::max(m_boundingBox.w, v.y);
		}
	}

	void Mesh::draw(DrawMode mode)
	{
		draw(mode, m_indices.size(), 0);
	}

	void Mesh::draw(DrawMode mode, int count, int offset)
	{
		glEnable(GL_PRIMITIVE_RESTART);
		initializeMesh();
		glBindVertexArray(m_vao);
		glDrawElements(static_cast<int>(mode), count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
		glBindVertexArray(0);
	}

	Math::vec4 Mesh::boundingBox()
	{
		return m_boundingBox;
	}

	std::unique_ptr<Mesh> Mesh::rotate(float rotation, Math::vec2 center)
	{
		auto vertices = m_vertices;

		for (auto &v : vertices)
		{
			v -= center;
			v = Math::rotate(v, rotation);
			v += center;
		}

		return std::unique_ptr<Mesh>(new Mesh(vertices, m_indices));
	}

	std::unique_ptr<Mesh> Mesh::scale(Math::vec2 scaling)
	{
		auto vertices = m_vertices;

		for (auto &v : vertices)
		{
			v *= scaling;
		}

		return std::unique_ptr<Mesh>(new Mesh(vertices, m_indices));
	}
}
