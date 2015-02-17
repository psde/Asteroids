#pragma once

#include <unordered_map>
#include <memory>

#include <GL/glew.h>

#include "Math/Math.h"

#include "Program.h"

namespace Graphics
{
	namespace Impl {
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

	class ShaderGlobals
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Impl::ShaderGlobalBase>> _globals;

		ShaderGlobals() {};
		
		static ShaderGlobals& instance()
		{
			static ShaderGlobals instance;
			return instance;
		}

	public:
		template<class T>
		static void update(std::string name, T val)
		{
			ShaderGlobals& globals = instance();

			std::shared_ptr<Impl::ShaderGlobalBase> &entry = globals._globals[name];

			if (!entry)
			{
				entry = std::make_shared<Impl::ShaderGlobal<T>>(name, val);
			}

			std::shared_ptr<Impl::ShaderGlobal<T>> global = std::dynamic_pointer_cast<Impl::ShaderGlobal<T>>(entry);
			global->updateValue(val);
		}

		static void updateProgram(Program *program)
		{
			ShaderGlobals& globals = instance();

			for (auto &global : globals._globals)
			{
				global.second->update(program);
			}
		}
	};
}