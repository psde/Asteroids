#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Shader.h"

namespace Graphics
{
	Shader::Shader(std::string name, GLuint type)
		: m_name(name), m_type(type)
	{
	}

	Shader::~Shader()
	{
		glDeleteShader(m_shader);
	}

	bool Shader::needsReload()
	{
		if (loadShaderfile(m_name) != m_content || loadShaderfile("data/shader/global.inc.glsl") != m_global)
		{
			return true;
		}
		return false;
	}

	std::string Shader::loadShaderfile(std::string filename)
	{
		std::ifstream stream(filename);
		if (stream.is_open() == false)
		{
			std::cerr << "Could not open file " << filename << std::endl;
			return "";
		}

		std::string fileContents((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		stream.close();
		return fileContents;
	}

	bool Shader::load()
	{
		m_global = loadShaderfile("data/shader/global.inc.glsl");
		m_content = loadShaderfile(m_name);

		std::stringstream source;
		source << "#version 330 core\n";

		switch (m_type)
		{
		case GL_GEOMETRY_SHADER:
			source << "#define GEOMETRY\n";
			break;
		case GL_VERTEX_SHADER:
			source << "#define VERTEX\n";
			break;
		case GL_FRAGMENT_SHADER:
			source << "#define FRAGMENT\n";
			break;

		default:
			std::cout << "Unknown shader type " << m_type << std::endl;
		}

		source << m_global << "\n";
		source << "#line 0\n";
		source << m_content;

		m_shader = glCreateShader(m_type);
		std::cout << m_type << " -> " << m_shader << std::endl;
		std::string s = source.str();
		const char *src = s.c_str();
		glShaderSource(m_shader, 1, &src, NULL);
		glCompileShader(m_shader);

		GLint isCompiled = 0;
		glGetShaderiv(m_shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 1000;
			glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(m_shader, maxLength, &maxLength, &errorLog[0]);

			std::cerr << "Failed to compile shader '" << m_name << "'" << std::endl << &errorLog[0] << std::endl;

			glDeleteShader(m_shader);
			return false;
		}

		return true;
	}
}
