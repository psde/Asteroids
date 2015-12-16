#pragma once

#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Graphics/Shader/Program.h"

namespace Graphics
{
	const unsigned int GeometryRestartIndex = 65535;
	enum class DrawMode
	{
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP
	};

	class Mesh
	{
	private:
		GLuint m_vao, m_vbo, m_ebo;

		bool m_initialized;

		std::vector<Math::vec2> m_vertices;
		std::vector<GLuint> m_indices;

		Math::vec4 m_boundingBox;

		void initializeMesh();
		void deleteMesh();
		void createBoundingBox();
	public:
		Mesh(const Mesh &other);
		Mesh(std::vector<Math::vec2> vertices);
		Mesh(std::vector<Math::vec2> vertices, std::vector<GLuint> indices);
		~Mesh();

		void draw(DrawMode mode);
		void draw(DrawMode mode, int count, int offset);

		Math::vec4 boundingBox();
		std::unique_ptr<Mesh> rotate(float rotation, Math::vec2 center);
		std::unique_ptr<Mesh> scale(Math::vec2 scaling);

		const std::vector<Math::vec2> &vertices() const { return m_vertices; }
	};
}
