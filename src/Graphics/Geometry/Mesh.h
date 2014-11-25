#pragma once

#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Graphics/Shader/Program.h"

namespace Geometry
{
	const unsigned int GeometryRestartIndex = 65535;
	enum DrawMode
	{
		POINTS = GL_POINTS,
		LINES = GL_LINES,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP
	};

	class Mesh
	{
	private:
		GLuint _vao, _vbo, _ebo;

		bool _initialized;

		std::vector<Math::vec2> _vertices;
		std::vector<GLuint> _indices;

		Math::vec4 _boundingBox;

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

		Math::vec4 getBoundingBox();
		std::unique_ptr<Mesh> rotate(float rotation, Math::vec2 center);
		std::unique_ptr<Mesh> scale(Math::vec2 scaling);

		const std::vector<Math::vec2> &vertices() const { return _vertices; }
	};
}