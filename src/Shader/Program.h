#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "UniformHandle.h"

namespace Shader
{
	namespace {
		class Shader
		{
		private:
			GLuint _shader;

			std::string _name;
			GLuint _type;
			std::string _content;

		public:
			Shader(std::string name, GLuint type);
			~Shader()
			{
				glDeleteShader(_shader);
			};

			bool load();
			bool needsReload();
			GLuint id() { return _shader; }
			std::string name() { return _name; }
			GLuint type() { return _type; }
		};
	}

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
