#pragma once

#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Shader/Program.h"

namespace Geometry
{
	class Mesh
	{
	private:
		GLuint _vao, _vbo, _ebo;

		bool _initialized;

		std::vector<glm::vec2> _vertices;
		std::vector<GLuint> _indices;

		void initializeMesh();
		void deleteMesh();
	public:
		Mesh(const Mesh &other);
		Mesh(std::vector<glm::vec2> vertices);
		Mesh(std::vector<glm::vec2> vertices, std::vector<GLuint> indices);
		~Mesh();
		
		void draw(GLenum mode);
		void draw(GLenum mode, int count, int offset);

		glm::vec4 getBoundingBox();
		std::unique_ptr<Mesh> rotate(float rotation, glm::vec2 center);
		std::unique_ptr<Mesh> scale(glm::vec2 scaling);
	};
}