#pragma once

#include <GL/glew.h>

#include <string>

namespace Shader
{
	class Shader
	{
	private:
		GLuint _shader;

		std::string _name;
		GLuint _type;
		std::string _content;

	public:
		Shader(std::string name, GLuint type);
		~Shader()
		{
			glDeleteShader(_shader);
		};

		bool load();
		bool needsReload();
		GLuint id() { return _shader; }
		std::string name() { return _name; }
		GLuint type() { return _type; }
	};
}