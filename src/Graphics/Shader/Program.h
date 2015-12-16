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

		GLuint m_shaderProgram;
		bool m_linked;

		bool addShader(GLuint type, std::string file);

		std::vector<std::shared_ptr<Shader>> m_shaders;
		std::map<std::string, GLuint> m_uniformLocations;

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
