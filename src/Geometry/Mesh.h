#pragma once

#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Shader/Program.h"

namespace Geometry
{
	class Mesh
	{
	public:
		enum DrawMode
		{
			POINTS = GL_POINTS,
			LINES = GL_LINES,
			LINE_STRIP = GL_LINE_STRIP,
			LINE_LOOP = GL_LINE_LOOP
		};

	private:
		GLuint _vao, _vbo, _ebo;

		bool _initialized;

		std::vector<glm::vec2> _vertices;
		std::vector<GLuint> _indices;

		glm::vec4 _boundingBox;

		void initializeMesh();
		void deleteMesh();
		void createBoundingBox();
	public:
		Mesh(const Mesh &other);
		Mesh(std::vector<glm::vec2> vertices);
		Mesh(std::vector<glm::vec2> vertices, std::vector<GLuint> indices);
		~Mesh();

		void draw(DrawMode mode);
		void draw(DrawMode mode, int count, int offset);

		glm::vec4 getBoundingBox();
		std::unique_ptr<Mesh> rotate(float rotation, glm::vec2 center);
		std::unique_ptr<Mesh> scale(glm::vec2 scaling);

		const std::vector<glm::vec2> &vertices() const { return _vertices; }
	};
}