#pragma once

#include <unordered_map>
#include <memory>

#include <GL/glew.h>

#include "Math/Math.h"

#include "Program.h"

namespace Graphics
{
	namespace {
		class ShaderGlobalBase
		{
		public:
			virtual void update(Program *program) = 0;
		};

		template<class T>
		class ShaderGlobal : public ShaderGlobalBase
		{
		private:
			std::string _name;
			T _val;

		public:
			ShaderGlobal(std::string name, T val) : _name(name), _val(val) { }

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
		std::unordered_map<std::string, std::shared_ptr<ShaderGlobalBase>> _globals;

		Globals() {};
	public:
		template<class T>
		void update(std::string name, T val)
		{
			std::shared_ptr<ShaderGlobalBase> &entry = _globals[name];

			if (!entry)
			{
				entry = std::make_shared<ShaderGlobal<T>>(name, val);
			}

			std::shared_ptr<ShaderGlobal<T>> global = std::dynamic_pointer_cast<ShaderGlobal<T>>(entry);
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