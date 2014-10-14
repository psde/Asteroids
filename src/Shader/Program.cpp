#include "Program.h"

#include <iostream>
#include <fstream>

#include "UniformHandle.h"

namespace Shader
{
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