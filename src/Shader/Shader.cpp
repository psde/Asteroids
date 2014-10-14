#include <iostream>
#include <fstream>
#include <vector>

#include "UniformHandle.h"

#include "Shader.h"

namespace Shader
{
	Shader::Shader(std::string name, GLuint type)
		:_name(name), _type(type)
	{
	}

	bool Shader::needsReload()
	{
		std::ifstream stream(_name);
		if (stream.is_open() == false)
		{
			std::cout << "Could not open file " << _name << std::endl;
			return false;
		}

		std::string fileContents((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		stream.close();

		if (fileContents != _content)
		{
			return true;
		}
		return false;
	}

	bool Shader::load()
	{
		std::ifstream stream(_name);
		if (stream.is_open() == false)
		{
			std::cout << "Could not open file " << _name << std::endl;
			return false;
		}

		_shader = glCreateShader(_type);

		std::string fileContents((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
		stream.close();

		_content = fileContents;

		std::string source("#version 150 core\n");

		switch (_type)
		{
		case GL_GEOMETRY_SHADER:
			source.append("#define GEOMETRY\n");
			break;
		case GL_VERTEX_SHADER:
			source.append("#define VERTEX\n");
			break;
		case GL_FRAGMENT_SHADER:
			source.append("#define FRAGMENT\n");
			break;

		default:
			std::cout << "Unknown shader type " << _type << std::endl;
		}

		source.append("#line 0\n");

		source.append(fileContents);

		const char *src = source.c_str();
		glShaderSource(_shader, 1, &src, NULL);
		glCompileShader(_shader);

		GLint isCompiled = 0;
		glGetShaderiv(_shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(_shader, maxLength, &maxLength, &errorLog[0]);

			std::cout << "Failed to compile shader" << std::endl << &errorLog[0] << std::endl;

			glDeleteShader(_shader);
			return false;
		}

		return true;
	}
}
