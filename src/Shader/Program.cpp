#include "Program.h"

#include "Globals.h"

#include <iostream>
#include <fstream>

namespace Shader
{
	Program::Program(std::string universalShader)
	{
		_linked = false;
		_shaderProgram = glCreateProgram();
		addUniversalShader(universalShader);
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

	void Program::addUniversalShader(std::string file)
	{
		addVertexShader(file);
		addFragmentShader(file);
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

		_linked = false;
		_shaderProgram = glCreateProgram();

		auto oldShaders = _shaders;
		_shaders.clear();
		for (auto shader : oldShaders)
		{
			addShader(shader->type(), shader->name());
		}

		link();

		_uniformLocations.clear();

		return true;
	}
	
	void Program::link()
	{
		if (_linked == false)
		{
			glLinkProgram(_shaderProgram);

			GLint isCompiled = 0;
			glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 1000;
				glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errorLog(maxLength);
				glGetProgramInfoLog(_shaderProgram, maxLength, &maxLength, &errorLog[0]);

				std::cout << "Failed to compile program" << std::endl << &errorLog[0] << std::endl;

				glDeleteProgram(_shaderProgram);
				return;
			}

			_linked = true;
		}
	}

	void Program::use()
	{
		static int tick = 0;
		tick++;

		if (tick > 2000) {
			reload();
			tick = 0;
		}

		link();
		glUseProgram(_shaderProgram);

		Globals::globals().updateProgram(this);
	}

	GLuint Program::getProgram()
	{
		link();
		return _shaderProgram;
	}


	UniformAssigner Program::uniform(const std::string& uniformName)
	{
		return (*this)[uniformName];
	}

	UniformAssigner Program::operator[](const std::string& uniformName)
	{
		GLuint location = -1;
		auto it = _uniformLocations.find(uniformName);
		if (it == _uniformLocations.end())
		{
			location = glGetUniformLocation(_shaderProgram, uniformName.c_str());
			_uniformLocations[uniformName] = location;
		}
		else
		{
			location = it->second;
		}

		return UniformAssigner(location);
	}
}