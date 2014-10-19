#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "UniformAssigner.h"
#include "Shader.h"

namespace Shader
{
	class Program;

	namespace
	{
		class GlobalBase
		{
		public:
			virtual void update(Program *program) = 0;
		};

		template<class T>
		class Global : public GlobalBase
		{
		private:
			T _val;
			std::string _name;

		public:
			Global(std::string name, T val) : _name(name), _val(val) { }

			void update(Program *program)
			{
				(*program)[_name] = _val;
			}
		};
	}

	class Globals
	{
	private:
		std::map<std::string, std::shared_ptr<GlobalBase>> _globals;

	public:
		Globals() {};

		template<class T>
		void update(std::string name, T val)
		{
			std::shared_ptr<Global<T>> global = std::make_shared<Global<T>>(name, val);
			_globals[name] = global;
		}

		void updateProgram(Program *program)
		{
			for (auto global : _globals)
			{
				global.second->update(program);
			}
		}
	};

	class Program
	{
	private:
		
		GLuint _shaderProgram;
		bool _linked;

		bool addShader(GLuint type, std::string file);

		std::vector<std::shared_ptr<Shader>> _shaders;
		std::map<std::string, GLuint> _uniformLocations;

		void link();

	public:
		Program();
		~Program();

		void addUniversalShader(std::string file);
		void addVertexShader(std::string file);
		void addFragmentShader(std::string file);

		void use();

		bool needsReload();
		bool reload();

		const GLuint getProgram();
		
		UniformAssigner operator[](const std::string& uniform_name);

		static Globals &globals()
		{
			static Globals globals;
			return globals;
		}
	};
}
