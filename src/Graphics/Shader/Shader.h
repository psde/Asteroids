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
		GLuint _shader;

		std::string _name;
		GLuint _type;
		std::string _content;
		std::string _global;

		std::string loadShaderfile(std::string filename);

		Shader(std::string name, GLuint type);
	public:
		~Shader();

		bool load();
		bool needsReload();
		GLuint id() { return _shader; }
		std::string name() { return _name; }
		GLuint type() { return _type; }
	};
}