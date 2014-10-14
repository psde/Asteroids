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
		std::map<std::string, GLuint*> _uniformLocations;

		void compile();
	public:
		Program();
		~Program();

		void addVertexShader(std::string file);
		void addFragmentShader(std::string file);

		void use();
		
		bool needsReload();
		bool reload();

		const GLuint getProgram();

		template<typename T>
		UniformHandle<T> createUniform(std::string name)
		{
			compile();

			auto it = _uniformLocations.find(name);
			if (it == _uniformLocations.end())
			{
				GLuint location = glGetUniformLocation(_shaderProgram, name.c_str());
				_uniformLocations[name] = new GLuint(location);
			}

			return UniformHandle<T>(_uniformLocations[name]);
		}
	};
}
