#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "UniformAssigner.h"
#include "Shader.h"

namespace Graphics
{
	class ShaderGlobals;

	class Program
	{
	private:

		GLuint _shaderProgram;
		bool _linked;

		bool addShader(GLuint type, std::string file);

		std::vector<std::shared_ptr<Shader>> _shaders;
		std::map<std::string, GLuint> _uniformLocations;

		void link();

		void addUniversalShader(std::string file);
		void addVertexShader(std::string file);
		void addFragmentShader(std::string file);

		Program(std::string universalFile);
	public:
		~Program();

		void use();

		bool needsReload();
		bool reload();

		GLuint getProgram();

		UniformAssigner uniform(const std::string& uniformName);

		UniformAssigner operator[](const std::string& uniformName);

		static std::shared_ptr<Program> getProgram(std::string universalFile);
	};
}
