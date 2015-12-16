#pragma once

#include <unordered_map>
#include <memory>

#include <GL/glew.h>

#include "Math/Math.h"

#include "Program.h"

namespace Graphics
{
	namespace Impl {
		class ShaderValueBase
		{
		public:
			virtual void update(Program *program) = 0;
		};

		template<class T>
		class ShaderValue : public ShaderValueBase
		{
		private:
			std::string m_name;
			T m_val;

		public:
			ShaderValue(std::string name, T val) : m_name(name), m_val(val) { }

			void updateValue(T val)
			{
				m_val = val;
			}

			void update(Program *program)
			{
				(*program)[m_name] = m_val;
			}
		};
	}

	class ShaderGlobals
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Impl::ShaderValueBase>> m_globals;

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

			std::shared_ptr<Impl::ShaderValueBase> &entry = globals.m_globals[name];

			if (!entry)
			{
				entry = std::make_shared<Impl::ShaderValue<T>>(name, val);
			}

			std::shared_ptr<Impl::ShaderValue<T>> global = std::dynamic_pointer_cast<Impl::ShaderValue<T>>(entry);
			global->updateValue(val);
		}

		static void updateProgram(Program *program)
		{
			ShaderGlobals& globals = instance();

			for (auto &global : globals.m_globals)
			{
				global.second->update(program);
			}
		}
	};
}