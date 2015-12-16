#pragma once

#include <GL/glew.h>

#include <string>

namespace Graphics
{
	class Program;

	class Shader
	{
		friend class Program;
	private:
		GLuint m_shader;

		std::string m_name;
		GLuint m_type;
		std::string m_content;
		std::string m_global;

		std::string loadShaderfile(std::string filename);

		Shader(std::string name, GLuint type);
	public:
		~Shader();

		bool load();
		bool needsReload();
		GLuint id() { return m_shader; }
		std::string name() { return m_name; }
		GLuint type() { return m_type; }
	};
}
