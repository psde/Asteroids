#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Shader.h"

namespace Graphics
{
	Shader::Shader(std::string name, GLuint type)
		:_name(name), _type(type)
	{
	}

	Shader::~Shader()
	{
		glDeleteShader(_shader);
	}

	bool Shader::needsReload()
	{
		if (loadShaderfile(_name) != _content || loadShaderfile("data/shader/global.inc.glsl") != _global)
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
			std::cout << "Could not open file " << filename << std::endl;
			return "";
		}

		std::string fileContents((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		stream.close();
		return fileContents;
	}

	bool Shader::load()
	{
		_global = loadShaderfile("data/shader/global.inc.glsl");
		_content = loadShaderfile(_name);

		std::stringstream source;
		source << "#version 330 core\n";

		switch (_type)
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
			std::cout << "Unknown shader type " << _type << std::endl;
		}

		source << _global << "\n";
		source << "#line 0\n";
		source << _content;

		_shader = glCreateShader(_type);
		std::string s = source.str();
		const char *src = s.c_str();
		glShaderSource(_shader, 1, &src, NULL);
		glCompileShader(_shader);

		GLint isCompiled = 0;
		glGetShaderiv(_shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 1000;
			glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(_shader, maxLength, &maxLength, &errorLog[0]);

			std::cout << "Failed to compile shader '" << _name << "'" << std::endl << &errorLog[0] << std::endl;

			glDeleteShader(_shader);
			return false;
		}

		return true;
	}
}
