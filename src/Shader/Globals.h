#pragma once

#include <unordered_map>
#include <memory>

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
		std::unordered_map<std::string, std::shared_ptr<GlobalBase>> _globals;

		Globals() {};
	public:
		template<class T>
		void update(std::string name, T val)
		{
			std::shared_ptr<GlobalBase> &entry = _globals[name];

			if (!entry)
			{
				entry = std::make_shared<Global<T>>(name, val);
			}

			std::shared_ptr<Global<T>> global = std::dynamic_pointer_cast<Global<T>>(entry);
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