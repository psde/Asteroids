#include "Program.h"

#include "Globals.h"

#include <iostream>
#include <fstream>

namespace Graphics
{
	Program::Program(std::string universalShader)
		: m_linked(false)
	{
		m_shaderProgram = glCreateProgram();
		addUniversalShader(universalShader);
	}

	Program::~Program()
	{
	}

	bool Program::addShader(GLuint type, std::string file)
	{
		std::shared_ptr<Shader> shader(new Shader(file, type));
		m_shaders.push_back(shader);

		if (shader->load())
		{
			glAttachShader(m_shaderProgram, shader->id());
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
		for (auto shader : m_shaders)
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

		m_linked = false;
		m_shaderProgram = glCreateProgram();

		auto oldShaders = m_shaders;
		m_shaders.clear();
		for (auto shader : oldShaders)
		{
			addShader(shader->type(), shader->name());
		}

		link();

		m_uniformLocations.clear();

		return true;
	}

	void Program::link()
	{
		if (m_linked == false)
		{
			glLinkProgram(m_shaderProgram);

			GLint isCompiled = 0;
			glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 1000;
				glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errorLog(maxLength);
				glGetProgramInfoLog(m_shaderProgram, maxLength, &maxLength, &errorLog[0]);

				std::cerr << "Failed to compile program" << std::endl << &errorLog[0] << std::endl;

				glDeleteProgram(m_shaderProgram);
				return;
			}

			m_linked = true;
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
		glUseProgram(m_shaderProgram);

		ShaderGlobals::updateProgram(this);
	}

	GLuint Program::getProgram()
	{
		link();
		return m_shaderProgram;
	}


	UniformAssigner Program::uniform(const std::string& uniformName)
	{
		return (*this)[uniformName];
	}

	UniformAssigner Program::operator[](const std::string& uniformName)
	{
		GLuint location = -1;
		auto it = m_uniformLocations.find(uniformName);
		if (it == m_uniformLocations.end())
		{
			location = glGetUniformLocation(m_shaderProgram, uniformName.c_str());
			m_uniformLocations[uniformName] = location;
		}
		else
		{
			location = it->second;
		}

		return UniformAssigner(location);
	}

	std::shared_ptr<Program> Program::getProgram(std::string universalFile)
	{
		static std::map<std::string, std::shared_ptr<Program>> programs;

		auto it = programs.find(universalFile);
		std::shared_ptr<Program> program;
		if (it == std::end(programs))
		{
			program.reset(new Program(universalFile));
			programs[universalFile] = program;
		}
		else
		{
			program = it->second;
		}
		return program;
	}
}
