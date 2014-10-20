#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Program.h"

namespace Shader
{
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
			std::string _name;
			T _val;

		public:
			Global(std::string name, T val) : _name(name), _val(val) { }

			void updateValue(T val)
			{
				_val = val;
			}

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

		Globals() {};
	public:
		template<class T>
		void update(std::string name, T val)
		{
			auto it = _globals.find(name);

			std::shared_ptr<Global<T>> global;
			if (it == std::end(_globals))
			{
				global = std::make_shared<Global<T>>(name, val);
				_globals[name] = global;
			}
			else
			{
				global = std::dynamic_pointer_cast<Global<T>>(it->second);
			}
			global->updateValue(val);
		}

		void updateProgram(Program *program)
		{
			for (auto global : _globals)
			{
				global.second->update(program);
			}
		}

		static Globals& globals()
		{
			static Globals globals;
			return globals;
		}
	};
}