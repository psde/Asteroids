#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Shader
{
	class UniformAssigner
	{
	private:
		GLuint _location;

	public:
		explicit UniformAssigner(GLuint location);

		UniformAssigner& operator=(const glm::mat4 &val);

		UniformAssigner& operator=(const glm::vec2 &val);

		UniformAssigner& operator=(const float &val);
	};
}