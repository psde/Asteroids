#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Program.h"

namespace Shader
{
	class Manager
	{
	private:

		Manager() {};
	public:
		static std::shared_ptr<Program> getProgram(std::string universalFile)
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
	};
}