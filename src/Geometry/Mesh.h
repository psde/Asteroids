#pragma once

#include <GL/glew.h>
#include <vector>
#include "Shader/Program.h"

namespace Geometry
{
	class Mesh
	{
	private:
		GLuint _vao, _vbo, _ebo;

		std::vector<GLfloat> _vertices;
		std::vector<GLuint> _indices;

		void initializeMesh();
	public:
		Mesh(std::vector<GLfloat> vertices);
		Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
		
		void draw(GLenum mode);
		void draw(GLenum mode, int count, int offset);
	};
}