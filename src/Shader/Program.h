#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "UniformAssigner.h"
#include "Shader.h"


namespace Shader
{
	class Globals;

	class Program
	{
	private:
		
		GLuint _shaderProgram;
		bool _linked;

		bool addShader(GLuint type, std::string file);

		std::vector<std::shared_ptr<Shader>> _shaders;
		std::map<std::string, GLuint> _uniformLocations;

		void link();

	public:
		Program();
		~Program();

		void addUniversalShader(std::string file);
		void addVertexShader(std::string file);
		void addFragmentShader(std::string file);

		void use();

		bool needsReload();
		bool reload();

		GLuint getProgram();
		
		UniformAssigner operator[](const std::string& uniform_name);

		static Globals& globals();
	};
}
