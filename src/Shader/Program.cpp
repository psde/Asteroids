#include "Program.h"

#include <iostream>
#include <fstream>

#include "UniformHandle.h"

namespace Shader
{
	namespace{
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


	Program::Program()
	{
		_compiled = false;
		_shaderProgram = glCreateProgram();
	}

	Program::~Program()
	{
	}

	bool Program::addShader(GLuint type, std::string file)
	{
		std::shared_ptr<Shader> shader(new Shader(file, type));
		_shaders.push_back(shader);

		if (shader->load())
		{
			glAttachShader(_shaderProgram, shader->id());
		}
		return true;
	}

	void Program::addVertexShader(std::string file)
	{
		addShader(GL_VERTEX_SHADER, file);
	}

	void Program::addFragmentShader(std::string file)
	{
		addShader(GL_FRAGMENT_SHADER, file);
	}

	bool Program::needsReload()
	{
		for (auto shader : _shaders)
		{
			if (shader->needsReload())
			{
				return true;
			}
		}
		return false;
	}

	bool Program::reload()
	{
		if (needsReload() == false)
			return false;

		_compiled = false;
		_shaderProgram = glCreateProgram();

		auto oldShaders = _shaders;
		_shaders.clear();
		for (auto shader : oldShaders)
		{
			addShader(shader->type(), shader->name());
		}

		compile();

		auto oldLocations = _uniformLocations;
		for (auto location : _uniformLocations)
		{
			*_uniformLocations[location.first] = glGetUniformLocation(_shaderProgram, location.first.c_str());
		}

		return true;
	}
	
	void Program::compile()
	{
		if (_compiled == false)
		{
			glLinkProgram(_shaderProgram);

			/*
			glGetProgramiv(_shaderProgram, GL_LINK_STATUS, (int *)&IsLinked);
			if (IsLinked == FALSE)
			{
				glGetProgramiv(_shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);

				shaderProgramInfoLog = (char *)malloc(maxLength);

				glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);

				free(shaderProgramInfoLog);
				return;
			}*/

			_compiled = true;
		}
	}

	void Program::use()
	{
		compile();
		glUseProgram(_shaderProgram);
	}

	const GLuint Program::getProgram()
	{
		compile();
		return _shaderProgram;
	}


}