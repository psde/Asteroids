#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "UniformHandle.h"
#include "Shader.h"

namespace Shader
{
	class Program
	{
	private:
		GLuint _shaderProgram;
		bool _compiled;

		bool addShader(GLuint type, std::string file);

		std::vector<std::shared_ptr<Shader>> _shaders;
		std::map<std::string, GLint*> _uniformLocations;

		void link();

	public:
		Program();
		~Program();

		void addVertexShader(std::string file);
		void addFragmentShader(std::string file);

		void use();
		
		bool needsReload();
		bool reload();

		const GLuint getProgram();

		UniformHandle createUniform(std::string name);
	};
}
